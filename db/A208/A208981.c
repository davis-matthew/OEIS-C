/*
 * A208981  Number of iterations required to reach a power of 2 in the 3x+1
 *          sequence starting at n.
 * https://oeis.org/A208981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 6170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A208981: number of Collatz iterations needed from n to hit a power of two. */
/* The algorithm iterates the Collatz map using an unsigned 64‑bit accumulator to avoid
   overflow for all n up to about 10000. The result is returned as seq_t. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Use a 64‑bit accumulator for safe intermediate calculations. */
    unsigned long long x = (unsigned long long)n;
    seq_t  count = 0;

    /* Stop immediately if n itself is a power of two. */
    while ((x & (x - 1)) != 0ULL)
    {
        if ((x & 1ULL) == 0ULL)   /* x is even */
            x >>= 1;
        else                       /* x is odd */
            x = 3ULL * x + 1ULL;

        ++count;
    }
    return count;
}
