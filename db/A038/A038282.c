/*
 * A038282  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*4^j.
 * https://oeis.org/A038282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 8^(i-j) * 4^j, where i =
 *            floor((sqrt(8*n+1)-1)/2), j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    while (((__int128)(i + 1) * (i + 2) / 2) <= n)
        ++i;

    /* Column j within that row. */
    idx_t j = n - (idx_t)((__int128)i * (i + 1) / 2);

    /* Compute the binomial coefficient C(i,j). */
    seq_t comb = binom(i, j);

    /* 8^(i-j) * 4^j = 2^(3*i - j). */
    idx_t exp = 3 * i - j;

    /* Compute 2^exp and multiply. */
    seq_t pow2_val = pow2(exp);

    return comb * pow2_val;
}
