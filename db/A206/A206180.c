/*
 * A206180  a(n) = Sum_{k=0..n} binomial(n,k)^3 * 3^k.
 * https://oeis.org/A206180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * 3^k
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 1;    /* term for k = 0 */
    seq_t binom = 1;    /* C(n,0) = 1 */
    seq_t pow3  = 1;    /* 3^0 = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update binomial coefficient C(n,k) from C(n,k-1) */
        seq_t top = (seq_t)(n - (k - 1)); /* n-k+1 */
        binom = (binom * top) / (seq_t)k; /* exact division */

        /* Update 3^k */
        pow3 *= 3;

        /* Compute term = binom^3 * pow3 */
        seq_t term = binom * binom;   /* binom^2 */
        term = term * binom;          /* binom^3 */
        term = term * pow3;           /* binom^3 * 3^k */

        /* Add to running sum */
        sum += term;
    }

    return sum;
}
