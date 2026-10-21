/*
 * A229213  Square array of denominators of t(n,k) = (1+1/(k*n))^n, read by
 *          descending antidiagonals.
 * https://oeis.org/A229213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, tabl, nonn
 * Formula:   a(n) is the denominator of (1+1/(k*n))^n where (n,k) are the row
 *            and column of index n read by descending antidiagonals of the
 *            square array. The denominator simplifies to (k*n)^n
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the antidiagonal sum S such that S*(S-1)/2 >= n */
    idx_t S = 2;
    /* Since n <= 10000 (typical), a simple loop is fast enough */
    while (S * (S - 1) / 2 < n) {
        ++S;
    }
    /* Number of terms before this antidiagonal */
    idx_t before = (S - 1) * (S - 2) / 2;
    /* Position inside this antidiagonal (1-based) */
    idx_t pos = n - before;
    idx_t row = pos;          /* n in t(n,k) */
    idx_t col = S - pos;      /* k in t(n,k) */

    /* Compute denominator = (row*col)^row */
    seq_t base = (seq_t)row * (seq_t)col;  /* k*n */
    seq_t exp  = (seq_t)row;               /* n */
    return big_pow(base, exp);
}
