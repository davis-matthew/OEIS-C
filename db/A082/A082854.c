/*
 * A082854  Integers 1 to Catalan(n) followed by integers 1 to Catalan(n+1)
 *          etc.
 * https://oeis.org/A082854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = value within a block defined by Catalan numbers. Let S(k)
 *            = sum_{i=0}^k Catalan(i). Find the smallest k with n < S(k).
 *            Then a(n) = (n - S(k-1)) + 1. The first term (n=0) starts the
 *            block for k = 0, which has length Catalan(0) = 1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t cumulative = 0;      /* S(k-1) at each iteration */
    idx_t cat = 1;             /* Catalan(k), starting with k = 0 */

    for (idx_t k = 0;; ++k)
    {
        /* If n lies inside the current block, return the position (1‑based). */
        if (n < cumulative + cat)
            return (seq_t)(n - cumulative) + (seq_t)1;

        cumulative += cat;     /* next block starts after current sum */

        /* Recurrence for Catalan numbers:
           C_{k+1} = C_k * (2*(2*k+1)) / (k+2)  (valid for k ≥ 0) */
        idx_t num = 2 * (2 * k + 1);
        cat = cat * num / (k + 2);
    }
}
