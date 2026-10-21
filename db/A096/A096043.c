/*
 * A096043  Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^9-M)/8,
 *          where M is the infinite lower Pascal's triangle matrix, 1<=k<=n.
 * https://oeis.org/A096043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows of T(r,k) = binom(r, k-1) *
 *            (9^{r-k+1} - 1) / 8 where r, k are the row and column indices
 *            (1‑based) such that n counts entries row‑major. The result is
 *            a(seq_t). Binomial coefficients and powers are computed with
 *            supplied helper routines binomial and big_pow
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* not used for the sequence (offset 1) */

    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* previous row count */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;                  /* column index (1‑based) */

    idx_t exp = r - c + 1;               /* exponent for 9^{exp} */

    /* binomial coefficient binom(r, c-1) */
    seq_t comb = binomial(r, c - 1);

    /* 9^{exp} as a seq_t */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)exp);

    /* (9^{exp} - 1) / 8  must be an integer */
    seq_t term = (pow9 - (seq_t)1) / (seq_t)8;

    return comb * term;
}
