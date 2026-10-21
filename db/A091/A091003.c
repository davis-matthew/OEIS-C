/*
 * A091003  Expansion of (1-3*x^2)/((1-2*x)*(1+3*x)).
 * https://oeis.org/A091003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (2^n + 4*(-3)^n + 5*0^n)/10
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The 0^n term contributes 5 only for n = 0. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Big‐integer type – use the provided big_pow routine. */
    if (sizeof(seq_t) > 8) {
        seq_t pow2    = big_pow((seq_t)2,     (seq_t)n);  /* 2^n   */
        seq_t powneg3 = big_pow((seq_t)(-3),  (seq_t)n);  /* (-3)^n*/
        seq_t num = pow2 + (seq_t)4 * powneg3;
        if (n == 0) num += (seq_t)5;         /* +5*0^0 */
        return num / (seq_t)10;
    }

    /* 8‑byte or smaller type – compute using 128‑bit intermediates
       (enough for all n whose result fits in seq_t). */
    __int128 pow2    = 1;   /* 2^n */
    __int128 powneg3 = 1;   /* (-3)^n */

    idx_t i;
    for (i = 0; i < n; ++i) {
        pow2    *= 2;
        powneg3 *= -3;
    }

    __int128 num = pow2 + (__int128)4 * powneg3;
    if (n == 0) num += 5;   /* +5*0^0 */

    /* Division by 10 is exact for all n. */
    return (seq_t)(num / 10);
}
