/*
 * A011631  Legendre symbol (n,251).
 * https://oeis.org/A011631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,251) = n^125 mod 251, mapped to -1, 0,
 *            1
 * Range:     i8 n <= 2, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Reduce n modulo 251 first */
    idx_t nmod = n % 251ULL;
    /* If n is divisible by 251, the Legendre symbol is 0 */
    if (nmod == 0) {
        return (seq_t)0;
    }
    /* Compute n^125 mod 251 */
    seq_t res = mod_pow((seq_t)nmod, 125ULL, (seq_t)251);
    /* Map the result to -1, 0, or 1 */
    if (res == (seq_t)1) {
        return (seq_t)1;
    }
    /* Since 251 is prime, res cannot be any other value except 250 (= -1 mod 251) */
    return (seq_t)-1;
}
