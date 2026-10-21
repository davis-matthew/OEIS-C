/*
 * A077459  Numbers k such that k and 3*k have the same digital binary sum.
 * https://oeis.org/A077459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A077459: numbers k such that popcount(k) == popcount(3*k) */
/* Computes the n‑th term by linear search. */

#include <stddef.h>

static inline int popcount_seq(seq_t x)
{
    int cnt = 0;
    while (x != (seq_t)0)
    {
        if ((x & 1) != 0)
            ++cnt;
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t k = 0;
    idx_t count = 0;
    while (1)
    {
        if (popcount_seq(k) == popcount_seq(k * 3))
        {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
