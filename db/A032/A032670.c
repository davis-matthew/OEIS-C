/*
 * A032670  Digit '7' concatenated with a(n) is a prime.
 * https://oeis.org/A032670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 482, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, isPrimeGeneric, pow10, seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th integer m such that the concatenation of digit 7
 * and m (i.e., 7…m in decimal) is a prime number. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static seq_t *seq = NULL;            /* array of found terms, 0‑based storage */
static size_t seq_cap = 0;           /* current allocated capacity */
static idx_t  seq_len = 0;           /* number of terms stored already */
static seq_t  next_candidate = (seq_t)1;  /* next integer to test */

/* Grow the internal cache if needed */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < seq_len; ++i)
        new_arr[i] = seq[i];
    if (seq)
        seq_free(seq);
    seq = new_arr;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined offset, return 0 */

    /* Acquire results up to n */
    while (seq_len < n) {
        /* Find next candidate whose 7‑prefixed number is prime */
        for (;;) {
            /* Build 7…cand */
            seq_t power = pow10(digit_len(next_candidate));
            seq_t concat = (seq_t)7 * power + next_candidate;
            if (isPrimeGeneric(concat)) {
                /* Store the candidate */
                ensure_capacity(seq_len + 1);
                seq[seq_len] = next_candidate;
                ++seq_len;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
    return seq[n - 1];
}
