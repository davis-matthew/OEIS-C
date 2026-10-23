/*
 * A025135  (n-1)st elementary symmetric function of binomial(n,0),
 *          binomial(n,1), ..., binomial(n,n).
 * https://oeis.org/A025135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e_{n-1}(binomial(n,0),binomial(n,1),...,binomial(n,n))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the (n‑1)st elementary symmetric function of the n‑th row
   of Pascal's triangle by iteratively multiplying (x+binomial(n,j))
   and keeping only the lowest three coefficients. */

seq_t a(idx_t n)
{
    /* coeff[k] holds the coefficient of x^k in the partial product
       after processing some of the binomial coefficients. */
    seq_t coeff[3] = {1, 0, 0};   /* start with 1 */

    seq_t cur = 1;                /* binomial(n,0) */
    for (idx_t j = 0; j <= n; ++j) {
        /* Multiply the current partial product by (x + cur) */
        seq_t new_c2 = coeff[2] * cur + coeff[1];
        seq_t new_c1 = coeff[1] * cur + coeff[0];
        seq_t new_c0 = coeff[0] * cur;

        coeff[0] = new_c0;
        coeff[1] = new_c1;
        coeff[2] = new_c2;

        if (j == n) break;       /* finished all coefficients */

        /* Compute the next binomial coefficient iteratively:
           binomial(n, j+1) = binomial(n, j) * (n-j) / (j+1) */
        cur = (cur * (seq_t)(n - j)) / (seq_t)(j + 1);
    }
    return coeff[2];               /* the coefficient of x^2 */
}
