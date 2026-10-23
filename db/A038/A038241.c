/*
 * A038241  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*11^j.
 * https://oeis.org/A038241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*4^(i-j)*11^j, where i is the largest
 *            integer with i*(i+1)/2 ≤ n, and j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find row i */
    idx_t i = 0;
    while (((__uint128_t)(i + 1) * (i + 2)) / 2 <= n) {
        ++i;
    }

    /* Column j */
    idx_t j = n - (idx_t)((( __uint128_t)i * (i + 1)) / 2);

    /* Compute entry: binomial(i,j) * 4^(i-j) * 11^j */
    seq_t term = binomial(i, j);
    term = seq_mul(term, pow_uint(4ULL, i - j));
    term = seq_mul(term, pow_uint(11ULL, j));

    return term;
}
