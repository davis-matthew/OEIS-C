/*
 * A202148  Sum of rows of the triangle in A080381.
 * https://oeis.org/A202148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} gcd(binomial(n, floor(n/2)), binomial(n, i))
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-07)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t central = binomial(n, n >> 1);
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; i++) {
        seq_t b = binomial(n, i);
        sum += gcd(central, b);
    }
    return sum;
}
