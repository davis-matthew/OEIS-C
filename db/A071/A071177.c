/*
 * A071177  Square-root of concatenation n'k, where k is minimal with n'k
 *          square (decimal notation).
 * https://oeis.org/A071177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function implements OEIS A071177: 
   For each n ≥ 1, a(n) is the smallest integer x such that
   the decimal representation of x² begins with the digits of n.
   Equivalently, a(n) = sqrt(concat(n,k)), where k is the minimal non‑negative
   integer whose concatenation with n forms a perfect square.
   Algorithm: for increasing digit lengths d of k, test the first few squares
   that could lie in the interval [n·10ᵈ , (n+1)·10ᵈ).  The first square found
   that satisfies the exact digit‑length condition yields the answer.
*/

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */
#include "../utils/pow10.h"        /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    /* n >= 1, but we do not rely on the offset in our loop */
    for (idx_t d = 1; ; ++d)
    {
        /* pow10_d = 10^d */
        seq_t pow10_d = pow10(d);

        /* low  = n * 10^d  ;  high = (n+1) * 10^d = low + 10^d */
        seq_t low  = (seq_t)n * pow10_d;
        seq_t high = low + pow10_d;

        /* start with the smallest integer x with x^2 >= low */
        seq_t x = floor_sqrt(low);
        if (x * x < low)
            ++x;  /* ceil sqrt(low) */

        /* examine the few squares that may lie in [low, high) */
        for ( ; x * x < high; ++x )
        {
            seq_t m = x * x;
            seq_t k = m - low;   /* the part after n in the concatenation */

            /* k must have exactly d digits.  For d > 1, it must be ≥ 10^(d‑1). */
            if (d == 1 || k >= (pow10_d / 10))
                return x;
        }
        /* if not found, continue with next digit length d */
    }
}
