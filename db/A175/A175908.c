/*
 * A175908  3*sum(k=1..n, floor(k^2/n)) - n^2.
 * https://oeis.org/A175908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*sum_{k=1}^{n} floor(k*k/n) - n*n
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against n == 0; not used but makes function safe */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk   = (seq_t)k * (seq_t)k;        /* k^2 as seq_t */
        seq_t q    = kk / n;                     /* floor(k^2 / n) */
        sum += q;
    }

    seq_t n_sq  = (seq_t)n * (seq_t)n;           /* n^2 as seq_t */
    return 3 * sum - n_sq;
}
