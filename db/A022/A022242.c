/*
 * A022242  Gaussian binomial coefficients [ n,2 ] for q = 8.
 * https://oeis.org/A022242
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((8^n - 1)*(8^(n-1) - 1))/441
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Gaussian binomial coefficient [n,2] for q = 8) */

#include "../utils/big_pow.h"

static inline seq_t pow8(idx_t n)
{
    /* For 64‑bit seq_t use 128‑bit intermediate to avoid overflow.
       For larger seq_t rely on the big‑integer routine.           */
    if (sizeof(seq_t) <= 8) {
        __uint128_t r = 1;
        for (idx_t i = 0; i < n; ++i)
            r *= 8;
        return (seq_t)r;
    } else {
        return big_pow((seq_t)8, (seq_t)n);
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0; /* undefined below the offset */

    seq_t term1 = pow8(n)     - (seq_t)1;
    seq_t term2 = pow8(n - 1) - (seq_t)1;

    if (sizeof(seq_t) <= 8) {
        /* Use 128‑bit product to keep the intermediate result exact. */
        __uint128_t prod = (__uint128_t)term1 * (__uint128_t)term2;
        prod /= 441;
        return (seq_t)prod;
    } else {
        return (term1 * term2) / (seq_t)441;
    }
}
