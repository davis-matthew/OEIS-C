/*
 * A187552  Triangle a(n,k) =
 *          binomial(n,k)*binomial(n+1,k+1)*binomial(n+2,k+2) read by rows.
 * https://oeis.org/A187552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(r,k) * binomial(r+1,k+1) * binomial(r+2,k+2)
 *            where r = floor( (sqrt(8*n+1)-1)/2 ) and k = n - r*(r+1)/2
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* compute r*(r+1)/2 without overflow using seq_t */
static inline idx_t triangular(idx_t r) {
    seq_t val = (seq_t)r * (seq_t)(r + 1) / 2;
    return (idx_t)val;
}

/* compute the sequence value for index n */
seq_t a(idx_t n) {
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (triangular(r + 1) <= n)
        r++;

    idx_t start = triangular(r);
    idx_t k = n - start;

    seq_t res = binomial(r, k);
    res *= binomial(r + 1, k + 1);
    res *= binomial(r + 2, k + 2);
    return res;
}
