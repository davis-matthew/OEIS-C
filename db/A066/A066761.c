/*
 * A066761  Number of positive integers of the form (n^2+k^2)/(n-k) for
 *          k=1,2,3,4,....,n-1.
 * https://oeis.org/A066761
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = count_{1<=k<=n-1} of ((n^2+k^2) mod (n-k) == 0) */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t count = 0;

    for (idx_t k = 1; k < n; ++k) {
        seq_t K = (seq_t)k;
        seq_t num = N2 + K * K;   /* n^2 + k^2 */
        seq_t den = N - K;        /* n - k */
        if (num % den == 0)
            ++count;
    }

    return count;
}
