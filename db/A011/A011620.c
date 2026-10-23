/*
 * A011620  Legendre symbol (n,191).
 * https://oeis.org/A011620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,191) = n^95 (mod 191)
 * Range:     i8 n <= 1, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n modulo 191 */
    idx_t r = n % 191;
    if (r == 0) {
        /* 0 is its own Legendre symbol */
        return (seq_t)0;
    }

    /* Compute n^95 mod 191 */
    seq_t base = (seq_t)r;
    seq_t res  = mod_pow(base, 95, 191);   /* res is between 0 and 190 */

    /* Map 1 → 1, 190 → -1 */
    if (res == 1) {
        return (seq_t)1;
    } else {
        return (seq_t)(-1);   /* res == 190, i.e. -1 mod 191 */
    }
}
