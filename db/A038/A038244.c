/*
 * A038244  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*2^j.
 * https://oeis.org/A038244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 5^(i-j) * 2^j where i is the unique
 *            integer with i*(i+1)/2 <= n < (i+1)*(i+2)/2 and j = n -
 *            i*(i+1)/2
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row index i (small enough for idx_t multiplication). */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;

    /* Compute the triangular entry. */
    seq_t result = binomial(i, j);
    result *= power5(i - j);
    result *= pow2(j);
    return result;
}
