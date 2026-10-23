/*
 * A130563  Fourth column (m=3) of the Laguerre-Sonin a=1/2 coefficient
 *          triangle.
 * https://oeis.org/A130563
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,3) * (2*n+1)!! / 105 , n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          i.e.  a(n) = C(n,3) * (2*n+1)!! / (7!!)   */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                      /* below offset, undefined */

    /* compute C(n,3) = n*(n-1)*(n-2)/6  */
    seq_t binom = (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2);
    binom /= 6;

    /* now multiply by (2*n+1)!! and divide by 105,
       but remove the factors 3, 5 and 7 as soon as they appear
       to keep intermediate values small. */
    seq_t res = binom;
    int div3 = 1, div5 = 1, div7 = 1;          /* flags for one‑time division */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t factor = (seq_t)(2 * k + 1);      /* odd factors from 1 to 2*n+1 */
        res *= factor;

        if (div3 && (factor % 3 == 0)) {        /* remove one factor of 3 */
            res /= 3;
            div3 = 0;
        }
        if (div5 && (factor % 5 == 0)) {        /* remove one factor of 5 */
            res /= 5;
            div5 = 0;
        }
        if (div7 && (factor % 7 == 0)) {        /* remove one factor of 7 */
            res /= 7;
            div7 = 0;
        }
    }

    return res;
}
