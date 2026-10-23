/*
 * A157529  Composite numbers whose first digit is equal to the sum of all the
 *          other digits.
 * https://oeis.org/A157529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 451, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: composite numbers whose first digit equals the sum of the other digits.
   The array is built on demand by checking successive integers for primeness and
   the digit condition. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) */

static seq_t *seq_all = NULL;   /* dynamic array of found terms */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* capacity of seq_all */

/* return true if the decimal representation of x satisfies:
   first digit == sum of the remaining digits */
static bool has_property(idx_t x)
{
    idx_t sum = 0;
    idx_t tmp = x;
    while (tmp > 0) {
        sum += tmp % 10;
        tmp /= 10;
    }
    idx_t first = x;
    while (first >= 10)
        first /= 10;
    return first == (sum - first);
}

/* append a value to the dynamic array, expanding it if necessary */
static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap < seq_len + 1)
            new_cap <<= 1;            /* ensure room for at least one more */

#ifdef __cplusplus
        seq_t *new_arr = new seq_t[new_cap];
        for (size_t i = 0; i < seq_cap; ++i)
            new_arr[i] = seq_all[i];
        delete[] seq_all;
        seq_all = new_arr;
#else
        seq_t *new_arr = (seq_t *)realloc(seq_all, new_cap * sizeof(seq_t));
        if (!new_arr) abort();       /* allocation failure */
        seq_all = new_arr;
#endif
        seq_cap = new_cap;
    }
    seq_all[seq_len++] = val;
}

/* generate terms until we have at least n entries */
static idx_t last_checked = 1;
static void ensure(idx_t n)
{
    if (seq_len >= n)
        return;

    for (idx_t cand = last_checked + 1; ; ++cand) {
        last_checked = cand;
        if (cand > 1 && !is_prime(cand)) {
            if (has_property(cand)) {
                append_seq((seq_t)cand);
                if (seq_len >= n)
                    break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    ensure(n);
    return seq_all[n - 1];
}
