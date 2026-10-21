/*
 * A066455  a(n) = 6*binomial(n,4) + 5*binomial(n,2) - 4*n + 5.
 * https://oeis.org/A066455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*binomial(n,4) + 5*binomial(n,2) - 4*n + 5
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 363, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binomial(n,2) */
    seq_t binom2 = 0;
    if (n >= 2) {
        binom2 = (seq_t)n * (n - 1) / 2;
    }
    /* 6*binomial(n,4) = n*(n-1)*(n-2)*(n-3)/4  (safe for n >= 4, else 0) */
    seq_t binom4_times_6 = 0;
    if (n >= 4) {
        binom4_times_6 = (seq_t)n * (n - 1) * (n - 2) * (n - 3) / 4;
    }
    /* Compute the value */
    seq_t result = binom4_times_6 + (seq_t)5 * binom2 - (seq_t)4 * n + 5;
    return result;
}
