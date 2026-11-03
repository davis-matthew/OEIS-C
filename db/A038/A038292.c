/*
 * A038292  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*2^j.
 * https://oeis.org/A038292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038292: Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*2^j,
   offset 0.  The sequence enumerates the triangle in antidiagonal order:
   a(0)=1, a(1)=9, a(2)=2, a(3)=81, … */
#include <stdbool.h>                /* for bool, may be unused */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Find row i so that i(i+1)/2 <= n < (i+1)(i+2)/2. */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        ++i;
    }

    idx_t row_start = i * (i + 1) / 2;
    idx_t j = n - row_start;               /* 0 <= j <= i */

    seq_t val = binomial(i, j);                /* binomial(i,j) */
    val = val * ipow_u64_idx(9, i - j);         /* 9^(i-j) */
    val = val * ipow_u64_idx(2, j);             /* 2^j */

    return val;
}
