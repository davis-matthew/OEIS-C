/*
 * A015368  Gaussian binomial coefficient [ n,8 ] for q=-11.
 * https://oeis.org/A015368
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..8} ((-11)^(n-i+1)-1)/((-11)^i-1)
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 8) return 0;          /* undefined below the offset */

    const idx_t k = 8;
    seq_t den[k];

    /* pre‑compute absolute denominator factors */
    seq_t powi = (seq_t)11;        /* 11^1 */
    for (idx_t i = 1; i <= k; ++i) {
        den[i-1] = (i & 1) ? (powi + 1) : (powi - 1);  /* |(-11)^i - 1| */
        powi *= (seq_t)11;        /* next power of 11 */
    }

    /* running product of numerators */
    seq_t prod = 1;

    /* numerator power starts at 11^n */
    seq_t pown = big_pow((seq_t)11, (seq_t)n);

    for (idx_t i = 1; i <= k; ++i) {
        /* absolute numerator factor for exponent e = n-i+1 */
        idx_t e = n - i + 1;
        seq_t term = (e & 1) ? (pown + 1) : (pown - 1);

        /* reduce term against remaining denominator factors */
        for (idx_t j = 0; j < k; ++j) {
            if (den[j] == 1) continue;
            seq_t g = gcd(term, den[j]);
            if (g > 1) {
                term /= g;
                den[j] /= g;
            }
        }

        prod *= term;  /* multiply reduced term */

        /* now try to cancel any remaining denominators */
        for (idx_t j = 0; j < k; ++j) {
            if (den[j] == 1) continue;
            if (prod % den[j] == 0) {
                prod /= den[j];
                den[j] = 1;
            }
        }

        pown /= (seq_t)11;   /* next power for lower exponent */
    }

    /* any denominator factors still left must divide prod */
    for (idx_t j = 0; j < k; ++j) {
        if (den[j] != 1)
            prod /= den[j];
    }

    return prod;
}
