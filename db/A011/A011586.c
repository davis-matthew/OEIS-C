/*
 * A011586  Legendre symbol (n,23).
 * https://oeis.org/A011586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Legendre symbol (n,23)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    const idx_t MOD = 23;
    idx_t v = n % MOD;
    if (v == 0)
        return 0;
    /* For prime 23, the Legendre symbol is 1 if n^11 ≡ 1 (mod 23),
       and -1 if n^11 ≡ 22 (mod 23). */
    seq_t pow = mod_pow((seq_t)v, 11ULL, MOD);
    return (pow == 1) ? 1 : -1;
}
