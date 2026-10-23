/*
 * A187545  Stirling transform (of the first kind) of the central Lah numbers
 *          (A187535).
 * https://oeis.org/A187545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Σ_{k=0}^n s(n,k) * L(k) (Stirling transform of the first
 *            kind applied to the central Lah numbers)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------- */
/*  When compiled as C++ (arbitrary precision seq_t) we rely on 
    the arithmetic operators of seq_t.                               */
/* --------------------------------------------------------------- */
#ifndef __cplusplus
/*  When compiled as plain C we use 128‑bit intermediates to avoid
    overflow for the 64‑bit build.  The code below works with
    any seq_t that is a plain unsigned integer type (8, 16, 32,
    64 or 128 bits).                                              */
typedef unsigned __int128 u128_t;
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* ----------------------------------------------------------- */
    /*  C++ implementation – uses seq_t operations directly.     */
    /* ----------------------------------------------------------- */
    static std::vector<seq_t> L, s;
    static idx_t cap = 0;
    if ((idx_t)(n + 1) > cap) {
        L.resize(n + 1);
        s.resize(n + 1);
        cap = n + 1;
    }

    /* central Lah numbers L(k) */
    L[0] = (seq_t)1;
    if (n >= 1) L[1] = (seq_t)2;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t t = (seq_t)(2 * k - 1);
        L[k] = ((L[k - 1] * (seq_t)4) * t * t) / (seq_t)k;
    }

    /* Stirling numbers of the first kind s(k) for current n */
    s[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) s[i] = (seq_t)0;

    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t k = m; k > 0; --k) {
            s[k] = s[k - 1] + (seq_t)(m - 1) * s[k];
        }
        s[0] = (seq_t)0;
    }

    /* compute a(n) = Σ s[k] * L[k] */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        result += s[k] * L[k];
    }
    return result;

#else
    /* ----------------------------------------------------------- */
    /*  Plain C implementation – uses 128‑bit intermediates.     */
    /* ----------------------------------------------------------- */
    static seq_t *L = NULL;
    static seq_t *s = NULL;
    static idx_t cap = 0;

    if ((idx_t)(n + 1) > cap) {
        L = (seq_t *)realloc(L, (n + 1) * sizeof(seq_t));
        s = (seq_t *)realloc(s, (n + 1) * sizeof(seq_t));
        cap = n + 1;
    }

    /* central Lah numbers L(k) */
    L[0] = (seq_t)1;
    if (n >= 1) L[1] = (seq_t)2;
    for (idx_t k = 2; k <= n; ++k) {
        u128_t t = (u128_t)(2 * k - 1);          /* 2k-1          */
        u128_t prod = (u128_t)L[k - 1];
        prod *= (u128_t)4;
        prod *= t;
        prod *= t;
        prod /= (u128_t)k;
        L[k] = (seq_t)prod;
    }

    /* Stirling numbers of the first kind s(k) */
    s[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) s[i] = (seq_t)0;

    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t k = m; k > 0; --k) {
            u128_t val = (u128_t)(m - 1) * (u128_t)s[k];
            val += (u128_t)s[k - 1];
            s[k] = (seq_t)val;
        }
        s[0] = (seq_t)0;
    }

    /* compute a(n) = Σ s[k] * L[k] */
    u128_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += (u128_t)s[k] * (u128_t)L[k];
    }
    return (seq_t)sum;
#endif
}
