/*
 * A088890  Polynexus numbers of order 8.
 * https://oeis.org/A088890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^8-(n-1)^8)-(n^2-(n-1)^2))/252
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 39, u64 n <= 925, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial(n, r) where 0 ≤ r ≤ 7.
   Uses cancellation to keep values within the range of seq_t. */
static seq_t binom_small(idx_t n, idx_t r)
{
    if (n < r) return 0;
    if (r == 0) return 1;

    /* r <= 7, so a fixed small array is sufficient */
    seq_t num[10];
    seq_t den[10];

    for (idx_t i = 0; i < r; ++i) {
        num[i] = (seq_t)(n - r + 1 + i);   /* n - r + 1 + i */
        den[i] = (seq_t)(i + 1);           /* i + 1     */
    }

    /* Cancel common factors between numerators and denominators */
    for (idx_t i = 0; i < r; ++i) {
        seq_t d = den[i];
        if (d == 1) continue;

        for (idx_t j = 0; j < r && d > 1; ++j) {
            seq_t g = gcd(num[j], d);
            if (g > 1) {
                num[j] /= g;
                d /= g;
            }
        }
    }

    seq_t res = 1;
    for (idx_t i = 0; i < r; ++i) {
        res *= num[i];
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Coefficients from the generating‑function expansion:
       1, 17, 62, 62, 17, 1  */
    static const seq_t coeffs[6] = {1, 17, 62, 62, 17, 1};

    seq_t total = 0;
    for (idx_t j = 0; j < 6; ++j) {
        idx_t k = n + 5 - j;                 /* n+5-j  */
        seq_t term = coeffs[j] * binom_small(k, 7);
        total += term;
    }
    return total;
}
