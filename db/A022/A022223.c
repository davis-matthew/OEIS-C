/*
 * A022223  Gaussian binomial coefficients [ n,5 ] for q = 6.
 * https://oeis.org/A022223
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..5} (6^(n-i+1)-1)/(6^i-1)
 * Range:     u8 none, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 191 (2026-11-01)
 * Helpers:   gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 5. Behaviour for n < 5 is irrelevant. */
    if (n < 5) return 0;

    seq_t result = (seq_t)1;

    /* compute factors i = 1..5 */
    for (idx_t i = 1; i <= 5; ++i)
    {
        /* denominator: 6^i - 1 */
        seq_t denom = ipow_idx((seq_t)6, i);
        denom = denom - (seq_t)1;

        /* numerator: 6^(n-i+1) - 1 */
        seq_t num = ipow_idx((seq_t)6, n - i + 1);
        num = num - (seq_t)1;

        /* reduce numerator and denominator by their GCD to keep numbers small */
        seq_t g = gcd(num, denom);
        num   = num / g;
        denom = denom / g;

        /* reduce current result and remaining denominator by their GCD */
        seq_t g2 = gcd(result, denom);
        result = result / g2;
        denom  = denom / g2;

        /* multiply the reduced numerator */
        result = seq_mul(result, num);

        /* divide by the reduced denominator (division is exact) */
        result = result / denom;
    }

    return result;
}
