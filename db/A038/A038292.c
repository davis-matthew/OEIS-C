/*
 * A038292  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*2^j.
 * https://oeis.org/A038292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038292: Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*2^j,
   offset 0.  The sequence enumerates the triangle in antidiagonal order:
   a(0)=1, a(1)=9, a(2)=2, a(3)=81, … */
#include <stdbool.h>                /* for bool, may be unused */
#include "../utils/binom.h"         /* seq_t binom(idx_t, idx_t); */
#include "../utils/pow_uint.h"      /* seq_t pow_uint(uint64_t, idx_t); */

seq_t a(idx_t n)
{
    /* Find row i so that i(i+1)/2 <= n < (i+1)(i+2)/2. */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        ++i;
    }

    idx_t row_start = i * (i + 1) / 2;
    idx_t j = n - row_start;               /* 0 <= j <= i */

    seq_t val = binom(i, j);                /* binomial(i,j) */
    val = val * pow_uint(9, i - j);         /* 9^(i-j) */
    val = val * pow_uint(2, j);             /* 2^j */

    return val;
}
