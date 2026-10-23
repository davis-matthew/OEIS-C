/*
 * A162535  A positive integer k is included if every length of the runs of
 *          0's and 1's in the binary representation of k is coprime to k.
 * https://oeis.org/A162535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 9960, u32 n <= 9960, u64 n <= 9960,
 *            u128 n <= 9960, big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162535:
 * a(n) is the n-th (n ≥ 1) positive integer k such that every run of
 * consecutive identical bits in the binary representation of k has a
 * length that is coprime to k.
 * The function builds the sequence lazily by scanning integers
 * in order and storing the ones that satisfy the property.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

/* Cached sequence values and allocation state. */
static seq_t *seq = NULL;      /* array of values */
static idx_t  seq_len = 0;     /* number of values stored  */
static idx_t  seq_cap = 0;     /* current allocated capacity */
static idx_t  next_k  = 1;     /* next integer to test */

/* Grow the array to hold at least 'need' elements. */
static void grow_seq(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
    seq = tmp;
    seq_cap = new_cap;
}

/* Test whether integer k satisfies the run‑length coprimality condition. */
static bool good(seq_t k)
{
    if (k == 0) return false;

    seq_t last = k & 1;   /* current bit value (0 or 1) */
    seq_t run  = 0;       /* length of the current run (seq_t for arbitrary size) */
    seq_t temp = k;

    while (temp > 0) {
        seq_t bit = temp & 1;
        if (bit == last) {
            ++run;
        } else {
            if (gcd(run, k) != 1) return false;
            last = bit;
            run  = 1;
        }
        temp >>= 1;
    }
    /* final run */
    if (gcd(run, k) != 1) return false;
    return true;
}

/* Return the n-th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    while (seq_len < n) {
        seq_t cand = (seq_t)next_k;
        if (good(cand)) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = cand;
        }
        ++next_k;
    }
    return seq[n - 1];
}
