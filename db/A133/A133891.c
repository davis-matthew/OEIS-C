/*
 * A133891  a(n) = binomial(n+p,n) mod p, where p=12.
 * https://oeis.org/A133891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+12,12) mod 12
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    const idx_t K = 12;
    /* Hold the numerator and denominator terms of the binomial coefficient */
    seq_t num[K], den[K];
    idx_t i, j;

    /* Initialize the arrays:
       num[i] = n + K - i   (for i = 0 .. K-1)
       den[i] = i + 1       (for i = 0 .. K-1)   */
    for (i = 0; i < K; ++i) {
        num[i] = (seq_t)(n + K - i);
        den[i] = (seq_t)(i + 1);
    }

    /* Cancel common factors between numerators and denominators
       to keep the numbers small.  The loops are tiny (K = 12). */
    for (i = 0; i < K; ++i) {
        seq_t di = den[i];
        if (di == (seq_t)1) continue;
        for (j = 0; j < K; ++j) {
            seq_t g = gcd(num[j], di);
            if (g > (seq_t)1) {
                num[j] /= g;
                di   /= g;
                den[i] = di;          /* keep updated value */
                if (di == (seq_t)1) break;
            }
        }
        den[i] = di;                  /* final denominator value */
    }

    /* Multiply the remaining numerator factors modulo 12. */
    seq_t res = (seq_t)1;
    const seq_t MOD = (seq_t)12;
    for (i = 0; i < K; ++i) {
        res = (res * (num[i] % MOD)) % MOD;
    }
    return res;
}
