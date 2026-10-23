/*
 * A111177  Number of base n numbers in which each digit appears at most once
 *          (all unnecessary 0's deleted).
 * https://oeis.org/A111177
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n-1} (i^2 + i + 1) * (n-1)!/(n-1-i)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 2 according to OEIS A111177 */
seq_t a(idx_t n)
{
    /* sequence is undefined for n < 2; return 0 */
    if (n < 2)
        return 0;

    idx_t m = n - 1;
    seq_t result = 0;
    seq_t perm = 1;          /* P(n-1, 0) = 1 */

    for (idx_t i = 0; i <= m; ++i) {
        /* term = (i^2 + i + 1) * perm
           use 128‑bit to avoid overflow for 64‑bit builds                     */
        unsigned __int128 term = (unsigned __int128)(i * i + i + 1) *
                                 (unsigned __int128)perm;

        /* accumulate result in the same 128‑bit space */
        result = (seq_t)((unsigned __int128)result + term);

        /* update perm for next i, except after last iteration */
        if (i < m)
            perm *= (m - i);
    }

    return result;
}
