/*
 * A169801  a(n) = ((n-1)^2*n^2*(n+1)^2)/6 -
 *          2*Sum_{l=2..n}Sum_{k=2..n}(n-k+1)*(n-l+1)*(k-1)*(l-1).
 * https://oeis.org/A169801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*binomial(n+4,6) + 36*binomial(n+3,6) +
 *            36*binomial(n+2,6) + 4*binomial(n+1,6)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom6(idx_t n)
{
    /* Return binomial(n,6); 0 if n<6.  Uses integer arithmetic only. */
    if (n < 6)
        return 0;
    seq_t res = 1;
    for (idx_t i = 0; i < 6; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t c1 = binom6(n + 4);  /* binomial(n+4,6) */
    seq_t c2 = binom6(n + 3);  /* binomial(n+3,6) */
    seq_t c3 = binom6(n + 2);  /* binomial(n+2,6) */
    seq_t c4 = binom6(n + 1);  /* binomial(n+1,6) */

    seq_t total =
        ((seq_t)4 * c1) + ((seq_t)36 * c2) + ((seq_t)36 * c3) + ((seq_t)4 * c4);

    return total;
}
