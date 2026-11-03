/*
 * A038306  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*4^j.
 * https://oeis.org/A038306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 10^(i - j) * 4^j where i =
 *            floor((sqrt(8*n + 1) - 1) / 2) (row number) j = n - i*(i + 1)/2
 *            (column number)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* --- find the row index i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 --- */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    /* --- compute the column index j --- */
    idx_t base = i * (i + 1) / 2;   /* number of terms before this row   */
    idx_t j = n - base;            /* position inside the row (0‑based) */

    /* --- compute the entry --- */
    seq_t val = binomial(i, j);     /* C(i,j) */
    val *= pow10(i - j);            /* 10^(i-j) */
    val *= pow4(j);                 /* 4^j     */
    return val;
}
