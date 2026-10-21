/*
 * A089799  Expansion of Jacobi theta function theta_2(q^(1/2))/q^(1/8).
 * https://oeis.org/A089799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is a triangular number (i.e. n = k(k+1)/2 for some
 *            k≥0), otherwise 0
 * Range:     u8 n <= 31, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Compute integer floor of sqrt(x) using binary search; avoids overflow by using division. */
static seq_t isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = low + ((high - low + 1) >> 1);
        if (mid == 0) {
            /* mid == 0 implies low stays 0; break to avoid infinite loop */
            low = 0;
            break;
        }
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    /* 8*n + 1, all in seq_t */
    seq_t t = ((seq_t)8) * ((seq_t)n) + (seq_t)1;

    /* Check if t is a perfect square */
    seq_t s = isqrt(t);
    if (s * s == t)
        return (seq_t)2;
    else
        return (seq_t)0;
}
