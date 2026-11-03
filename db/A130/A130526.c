/*
 * A130526  A permutation of the integers induced by the lower and upper
 *          Wythoff sequences.
 * https://oeis.org/A130526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = k if n = floor(k*φ) (lower Wythoff), a(n) = -k if n = k +
 *            floor(k*φ) (upper Wythoff)
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t floor_k_phi(idx_t k)
{
    /*  φ = (1 + √5) / 2  */
    /*  Use scaled integer arithmetic:  φ ≈ (SCALE + √5_scaled) / (2·SCALE)  */
    const idx_t SCALE          = ((idx_t)1 << 32);          /* 2^32  */
    const idx_t SQRT5_SCALED   = 9606324949ULL;             /* floor( √5 · 2^32 )  */

    unsigned __int128 num = (unsigned __int128)k *
                            ((unsigned __int128)SCALE + SQRT5_SCALED);
    unsigned __int128 den = (unsigned __int128)2 * SCALE;
    return (seq_t)(num / den);      /* floor(k * φ)  */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    for (idx_t k = 1;; ++k)
    {
        seq_t l = floor_k_phi(k);          /* lower Wythoff term  */
        if ((seq_t)n == l) return (seq_t)k;

        seq_t u = l + (seq_t)k;            /* upper Wythoff term  */
        if ((seq_t)n == u) return -(seq_t)k;

        /* Both sequences have surpassed n → n must have been found already */
        if (l > n && u > n) return 0;
    }
}
