/*
 * A151758  G.f.: Theta^2-Theta, where Theta = Sum_{k>=0} x^(2^k).
 * https://oeis.org/A151758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A073267(n) - A209229(n)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts ordered pairs of powers of two that sum to n (A073267)
   minus 1 if n itself is a power of two (A209229). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset 0 → first term is a(0) = 0 */
    if (n == 0)
        return 0;

    /* Count i such that n - 2^i is a positive power of two */
    int count = 0;

    /* Compute floor(log2(n)) without calling any helper */
    idx_t tmp = n;
    idx_t maxi = 0;
    while (tmp > 1) {
        tmp >>= 1;
        maxi++;
    }

    for (idx_t i = 0; i <= maxi; ++i) {
        idx_t p = 1ULL << i;          /* 2^i */
        if (p > n) break;             /* safety */
        idx_t m = n - p;
        if (m && ((m & (m - 1)) == 0))
            count++;
    }

    /* Subtract 1 if n itself is a power of two */
    if ((n & (n - 1)) == 0)
        count--;

    return (seq_t)count;
}
