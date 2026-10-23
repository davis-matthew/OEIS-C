/*
 * A038308  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*6^j.
 * https://oeis.org/A038308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*10^(i-j)*6^j, where * i =
 *            floor((sqrt(8*n+1)-1)/2) * j = n - i*(i+1)/2 * (OEIS A038308)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* Find row index i: maximum i such that i*(i+1)/2 <= n */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        ++i;
    }
    /* Previous total terms before row i */
    idx_t prev = i * (i + 1) / 2;
    /* Column index within the row */
    idx_t j = n - prev;

    seq_t val = binomial(i, j);
    /* Multiply by 10^(i-j) */
    val *= pow_uint(10, i - j);
    /* Multiply by 6^j */
    val *= pow_uint(6, j);

    return val;
}
