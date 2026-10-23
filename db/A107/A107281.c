/*
 * A107281  a(0) = 1, a(1) = 1, a(2) = 2 and for n >= 1: a(n+1) = SORT[a(n) +
 *          a(n-1) + a(n-2)] where SORT places digits in ascending order and
 *          deletes 0's.
 * https://oeis.org/A107281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(n+1)=SORT[a(n)+a(n-1)+a(n-2)], where
 *            SORT puts decimal digits in ascending order and removes 0's
 * Range:     u8 n <= 20, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   sort_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sort_digits.h"

/* Static storage for memoized terms */
static seq_t *seq = NULL;
static idx_t seq_len = 0;
static idx_t seq_cap = 0;

/* Resize the memoization array if needed */
static void ensure_cap(idx_t needed)
{
    if (needed <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* Handle the first three terms directly */
        switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        default: break; /* unreachable */
        }
    }

    /* Initialize memoization if not yet done */
    if (seq_len == 0) {
        ensure_cap(3);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)2;
        seq_len = 3;
    }

    /* Compute missing terms up to n */
    if (n >= seq_len) {
        ensure_cap(n + 1);
        for (idx_t i = seq_len; i <= n; ++i) {
            seq_t a_n   = seq[i - 1];
            seq_t a_n1  = seq[i - 2];
            seq_t a_n2  = seq[i - 3];
            seq_t sum   = a_n + a_n1 + a_n2;
            seq_t sorted = sort_digits(sum);
            seq[i] = sorted;
        }
        seq_len = n + 1;
    }

    return seq[n];
}
