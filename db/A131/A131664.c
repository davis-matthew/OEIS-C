/*
 * A131664  A string of n 1's repeated n times.
 * https://oeis.org/A131664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = repunit of length k where k is the smallest integer with
 *            * k(k+1)/2 ≥ n (equivalently, k = ceil((sqrt(8*n+1)-1)/2)). The
 *            value * returned is the integer consisting of k consecutive
 *            decimal 1’s
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 55, u64 n <= 210, u128 n <= 780,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The function works for any n ≥ 1. It uses only integer arithmetic and
 * loops that run in O(√n) time, which is far below the required time
 * limits for n up to 10 000.  It does not depend on any external helper
 * functions. */
seq_t a(idx_t n)
{
    /* Find k such that T_{k-1} < n ≤ T_k, where T_k = k(k+1)/2. */
    idx_t k = 1;
    idx_t tri = 1;           /* tri = T_k */
    while (tri < n) {
        ++k;
        tri += k;            /* accumulates the next triangular number */
    }

    /* Build the repunit of length k: 111...1 (k ones). */
    seq_t rep = 0;
    for (idx_t i = 0; i < k; ++i) {
        rep = rep * 10 + 1;
    }
    return rep;
}
