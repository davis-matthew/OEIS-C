/*
 * A221014  The Wiener index of the dendrimer G_n , defined pictorially in the
 *          Ashrafi - Shabani - Diudea reference.
 * https://oeis.org/A221014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -666 + 2^n*(-270*n+2340) + 4^n*(972*n-1674)
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small, pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"
#include "../utils/mul_small.h"

/* Computes A221014  (Wiener index of the dendrimer G_n). */
seq_t a(idx_t n)
{
    /* Positive and negative contributions accumulator */
    seq_t pos = 0;
    seq_t neg = 0;

    /* Pre‑compute the powers of 2 and 4 */
    seq_t pow2_n = pow2(n);   /* 2^n  */
    seq_t pow4_n = pow4(n);   /* 4^n  */

    /* coeff for the 2^n term :  (-270*n + 2340)   (may be negative) */
    long long coeff1 = -270LL * (long long)n + 2340LL;

    if (coeff1 >= 0) {
        pos += mul_small(pow2_n, (unsigned)coeff1);
    } else {
        neg += mul_small(pow2_n, (unsigned)(-coeff1));
    }

    /* coeff for the 4^n term :  (972*n - 1674)   (may be negative) */
    long long coeff2 = 972LL * (long long)n - 1674LL;

    if (coeff2 >= 0) {
        pos += mul_small(pow4_n, (unsigned)coeff2);
    } else {
        neg += mul_small(pow4_n, (unsigned)(-coeff2));
    }

    /* subtract the constant 666 */
    neg += (seq_t)666;

    /* The result is always non‑negative, so plain subtraction is safe. */
    return pos - neg;
}
