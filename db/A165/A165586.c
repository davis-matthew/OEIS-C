/*
 * A165586  Legendre symbol (n,514229).
 * https://oeis.org/A165586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,514229)
 * Range:     i8 n <= 3, i16 n <= 3, i32 n <= 9342, i64 n <= 9342,
 *            i128 n <= 9342, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Special case: (0, p) = 0 */
    if (n == 0)
        return 0;

    /* Prime modulus 514229 */
    const uint64_t P = 514229ULL;
    /* Compute n mod P to keep modular exponent small */
    uint64_t r = n % P;
    if (r == 0)
        return 0;

    /* Euler's criterion: n^((p-1)/2) mod p */
    const uint64_t EXP = (P - 1) / 2;             /* 257114 */
    seq_t val = mod_pow((seq_t)r, EXP, (seq_t)P);

    /* Resolve Legendre symbol value */
    if ((long long)val == 1)
        return 1;
    /* If the result equals p-1, then Legendre symbol is -1 */
    else
        return -1;
}
