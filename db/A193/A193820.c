/*
 * A193820  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(x+1)^n and q(n,x)=x^n+x^(n-1)+...+x+1.
 * https://oeis.org/A193820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = sum_{j=0}^{k} binomial(n-1, j) for 0 <= k <= n. The
 *            sequence a(n) enumerates the array row‑wise, where the r‑th row
 *            (r starting at 0) has r+1 elements and its first index is
 *            offset(r) = r*(r+1)/2
 * Range:     u8 n <= 48, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t etc. (used via typedefs seq_t, idx_t) */

seq_t a(idx_t n) {
    /* Find row r such that offset(r) <= n < offset(r+1) */
    idx_t r = 0;
    while ( (r+1)*(r+2)/2 <= n ) {
        r++;
    }
    /* Index within the row */
    idx_t offset = r*(r+1)/2;
    idx_t k = n - offset;          /* 0 <= k <= r */

    /* Special case: first row (r == 0) contains only the value 1. */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Compute prefix sum of binomial coefficients for the (r-1)-th row. */
    seq_t sum = (seq_t)1;          /* binomial(r-1, 0) */
    seq_t binom = (seq_t)1;        /* current binomial coefficient */

    idx_t n_minus_1 = r - 1;       /* n-1 in the combinatorial formula */

    /* Add binom(r-1, j) for j = 1 .. k.
       binom(r-1, j) = binom(r-1, j-1) * (n-1-(j-1)) / j  */
    for (idx_t j = 1; j <= k; ++j) {
        binom = binom * (seq_t)(n_minus_1 - (j-1));
        binom = binom / (seq_t)j;
        sum += binom;
    }

    return sum;
}
