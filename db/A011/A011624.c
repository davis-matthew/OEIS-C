/*
 * A011624  Legendre symbol (n,211).
 * https://oeis.org/A011624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) == n^105 (mod 211) (Legendre symbol (n|211))
 * Range:     i8 n <= 3, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Legendre symbol is zero when 211 divides n */
    if ((n % 211ULL) == 0ULL) {
        return 0;
    }

    /* n^105 mod 211 is either 1 or 210 (= -1). */
    seq_t r = mod_pow((seq_t)n, 105ULL, (seq_t)211);

    if (r == (seq_t)1) {
        return 1;
    } else {
        return -1;
    }
}
