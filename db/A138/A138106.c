/*
 * A138106  A triangular sequence of coefficients based on the expansion of a
 *          Morse potential type function: p(x,t) = exp(x*t)*(exp(-2*t) -
 *          2*exp(-t)).
 * https://oeis.org/A138106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = binom(m,j)*(((-2)^(m-j)-2*(-1)^(m-j))) where m = row
 *            number of n in the flattened triangle, j = position of element
 *            within that row (0‑based)
 * Range:     i8 n <= 22, i16 n <= 69, i32 n <= 258, i64 n <= 915,
 *            i128 n <= 3509, big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

static inline idx_t find_row(idx_t idx) {
    idx_t m = 0;
    while (( (__uint128_t)(m + 1) * (m + 2) / 2 ) <= idx) {
        ++m;
    }
    return m;
}

seq_t a(idx_t n) {
    /* n >= 1 for valid terms; behaviour for n == 0 is undefined */
    idx_t idx = n - 1;                    /* 0‑based linear index */
    idx_t m   = find_row(idx);            /* row (also polynomial degree) */
    idx_t start = ( (__uint128_t)m * (m + 1) / 2 ) + 1; /* 1‑based start of row */
    idx_t k = n - start;                  /* position within the row (0‑based) */
    idx_t e = m - k;                      /* exponent in the inner expression */

    seq_t bin = binom(m, k);              /* binomial coefficient binom(m,k) */

    seq_t pow2 = big_pow((seq_t)2, (seq_t)e); /* 2^e  (seq_t type) */
    seq_t inner = pow2 - (seq_t)2;              /* 2^e - 2          */
    if (e & 1) inner = -inner;                  /* apply (-1)^e factor */

    return bin * inner;                      /* final coefficient */
}
