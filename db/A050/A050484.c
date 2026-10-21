/*
 * A050484  Partial sums of A051946.
 * https://oeis.org/A050484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6,6)*(5*n+7)/7
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 59, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute binomial(n+6,6) iteratively to avoid large intermediates */
    seq_t binom = 1;
    seq_t seq_n = (seq_t)n;
    for (idx_t i = 1; i <= 6; ++i)
    {
        seq_t num = seq_n + 6 - (i - 1);   /* (n+6)-(i-1) = n+7-i */
        binom = binom * num;
        binom = binom / i;                /* exact division; binomial stays integer */
    }

    seq_t part = ((seq_t)5 * seq_n + 7);   /* (5*n+7) */
    seq_t result = (binom * part) / 7;     /* division exact */
    return result;
}
