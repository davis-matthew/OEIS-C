/*
 * A022232  Gaussian binomial coefficients [ n,3 ] for q = 7.
 * https://oeis.org/A022232
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..3} (7^(n-i+1)-1)/(7^i-1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 196 (2026-11-01)
 * Helpers:   gcd, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* a(3) is the sequence offset; behaviour for n < 3 is irrelevant */
    if (n < 3)
        return 0;

    seq_t res = 1;

    for (idx_t i = 1; i <= 3; ++i)
    {
        seq_t num = ipow_u64_idx(7, n - i + 1) - 1;   /* 7^(n-i+1) - 1 */
        seq_t den = ipow_u64_idx(7, i) - 1;           /* 7^i - 1 */

        /* Simplify the fraction num/den */
        seq_t g = gcd(num, den);
        if (g != 1) { num /= g; den /= g; }

        /* Cancel any common factor of den with the running result */
        g = gcd(res, den);
        if (g != 1) { res /= g; den /= g; }

        /* Multiply the reduced numerator, then divide by any remaining den */
        res *= num;
        if (den != 1)
            res /= den;   /* division is exact */
    }

    return res;
}
