/*
 * A038246  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*4^j.
 * https://oeis.org/A038246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 5^(i-j) * 4^j where (i,j) is the position
 *            in the triangle T(i,j) = C(i,j)·5^(i-j)·4^j enumerated along
 *            antidiagonals: 0 → (0,0), 1 → (1,0), 2 → (1,1), 3 → (2,0),
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find row i by solving (i+1)(i+2)/2 > n */
    idx_t i = 0;
    idx_t cum = 1;            /* cumulative count up to row i (inclusive) */
    while (cum <= n) {
        ++i;
        cum = (i + 1) * (i + 2) / 2;
    }

    /* j is the column index within row i */
    idx_t prev = i * (i + 1) / 2;  /* cumulative count up to previous row */
    idx_t j = n - prev;

    seq_t bin = binomial(i, j);   /* C(i,j) */
    seq_t pow5 = pow_seq(5, i - j);   /* 5^(i-j) */
    seq_t pow4 = pow_seq(4, j);       /* 4^j */

    return bin * pow5 * pow4;
}
