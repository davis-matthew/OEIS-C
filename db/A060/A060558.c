/*
 * A060558  Fifth column (m=4) of triangle A060556.
 * https://oeis.org/A060558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 18*n + 35)*binomial(n+6, 6)/7
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 38, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Multiply and divide by 7 carefully to avoid overflow on small seq_t */
seq_t a(idx_t n)
{
    /* compute the polynomial coefficient */
    seq_t coeff = (seq_t)2 * (seq_t)n * (seq_t)n
               + (seq_t)18 * (seq_t)n + 35;

    /* compute the binomial coefficient */
    seq_t bin = binomial(n + 6, 6);

    /* decide how to divide by 7 safely */
    if ((coeff % 7) == 0) {
        return (coeff / 7) * bin;
    } else if ((bin % 7) == 0) {
        return coeff * (bin / 7);
    } else {
        /* neither factor divisible by 7; use a 128‑bit intermediate
           when available, otherwise fall back to seq_t arithmetic. */
#if defined(__SIZEOF_INT128__) && !defined(__GNUC__) || defined(__clang__)
        /* GCC/Clang support __uint128_t; cast operands safely. */
        __uint128_t prod = (__uint128_t)coeff * (__uint128_t)bin;
        prod /= 7;
        return (seq_t)prod;
#else
        /* Fallback – compute product then divide in seq_t. */
        seq_t prod = coeff * bin;
        return prod / 7;
#endif
    }
}
