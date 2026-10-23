/*
 * A174732  Triangle T(n, k, q) = (1-q^n)*(1/k)*binomial(n-1, k-1)*binomial(n,
 *          k-1) - (1-q^n) + 1, for q = 3, read by rows.
 * https://oeis.org/A174732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 59, i64 n <= 217, i128 n <= 757,
 *            big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   Let r be the row such that r(r+1)/2 ≥ n and (r-1)r/2 < n.
 *   Let k = n - (r-1)r/2  (1‑based column in row r).
 *   T(r,k,3) = (1-3^r)*(1/k)*C(r-1,k-1)*C(r,k-1) - (1-3^r) + 1
 *   a(n) = T(r,k,3)   (read triangle by rows)
 */
#include <stdbool.h>           /* for bool if needed */
#include "../utils/pow3.h"     /* seq_t pow3(idx_t exp);             */
#include "../utils/binom.h"   /* seq_t binom(idx_t n, idx_t k);    */

seq_t a(idx_t n)
{
    /* Find the row r and column k for n (1‑based). */
    idx_t r = 1;
    /* Triangular number r(r+1)/2 */
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Number of terms before row r */
    idx_t prev = (r * (r - 1)) / 2;
    idx_t k = n - prev;                 /* 1‑based column */

    /* Compute components of the formula */
    seq_t one = (seq_t)1;
    seq_t p   = pow3(r);                /* 3^r */
    seq_t one_minus_p = one - p;        /* 1 - 3^r */

    /* Binomial factors */
    seq_t bin1 = binom(r - 1, k - 1);
    seq_t bin2 = binom(r,     k - 1);

    /* (1/k)*binom(r-1,k-1)*binom(r,k-1) */
    seq_t prod = bin1 * bin2 / (seq_t)k;

    /* Apply the formula: (1-3^r)*(prod-1)+1 */
    seq_t val = one_minus_p * (prod - one) + one;

    return val;
}
