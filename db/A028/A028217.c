/*
 * A028217  Expansion of 1/((1-6x)(1-9x)(1-10x)(1-12x)).
 * https://oeis.org/A028217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*12^(n+3)-9*10^(n+3)+8*9^(n+3)-6^(n+3))/72 (OEIS
 *            A028217, offset 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#include <stdlib.h>
#endif

#if defined(__GNUC__) || defined(__clang__)
  // Helper for arbitrary‑precision bases (used only when seq_t is large)
  #include "../utils/big_pow.h"
#endif

static inline unsigned __int128
pow_u128(unsigned __int128 base, idx_t exp) {
    unsigned __int128 res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    /* The size of seq_t determines the computation path.
       For 8‑byte or smaller types we rely on 128‑bit arithmetic.
       For larger seq_t (e.g. arbitrary precision) we use the
       big_pow routine. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate path */
        unsigned __int128 exp   = (unsigned __int128)(n + 3);
        unsigned __int128 pow12 = pow_u128(12, exp);
        unsigned __int128 pow10 = pow_u128(10, exp);
        unsigned __int128 pow9  = pow_u128(9,  exp);
        unsigned __int128 pow6  = pow_u128(6,  exp);

        unsigned __int128 tmp1 = 2 * pow12 + 8 * pow9;
        unsigned __int128 tmp2 = 9 * pow10 + pow6;
        unsigned __int128 diff = tmp1 - tmp2;
        unsigned __int128 res  = diff / 72;

        return (seq_t)res;
    } else {
        /* arbitrary‑precision path: use big_pow and seq_t arithmetic */
        seq_t exp = (seq_t)(n + 3);

        seq_t pow12 = big_pow((seq_t)12, exp);
        seq_t pow10 = big_pow((seq_t)10, exp);
        seq_t pow9  = big_pow((seq_t)9,  exp);
        seq_t pow6  = big_pow((seq_t)6,  exp);

        seq_t tmp1 = (seq_t)2 * pow12 + (seq_t)8 * pow9;
        seq_t tmp2 = (seq_t)9 * pow10 + pow6;
        seq_t diff = tmp1 - tmp2;
        return diff / (seq_t)72;
    }
}
