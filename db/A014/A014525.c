/*
 * A014525  Order of shuffle group for deck of 3n cards.
 * https://oeis.org/A014525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; if n is a power of 3 (n=3^m) then a(n) = (m+1) *
 *            6^(m+1); else if n is divisible by 4 then a(n) = (3*n)! / 2;
 *            else a(n) = (3*n)!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* check if n is a power of 3 */
    idx_t m = 0;
    idx_t p = 1;
    while (p < n) {
        /* avoid overflow, but for n up to 10000 this is safe */
        if (p > n / 3) break;
        p *= 3;
        m++;
    }
    if (p == n) {
        /* n = 3^m, compute (m+1)*6^(m+1) */
        idx_t exp = m + 1;
        seq_t sixpow = 1;
        for (idx_t i = 0; i < exp; ++i) sixpow *= (seq_t)6;
        return ((seq_t)(m + 1)) * sixpow;
    }

    /* otherwise, compute factorial of 3*n */
    seq_t fac = factorial(3 * n);
    if ((n & 3) == 0) {        /* n divisible by 4 */
        return fac / 2;         /* factorial is even */
    }
    return fac;
}
