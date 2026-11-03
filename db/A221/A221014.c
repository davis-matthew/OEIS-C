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
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
