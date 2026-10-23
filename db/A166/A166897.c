/*
 * A166897  a(n) = Sum_{k=0..[n/2]} C(n-k,k)^3*n/(n-k), n>=1.
 * https://oeis.org/A166897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..[n/2]} C(n-k,k)^3*n/(n-k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c   = 1;            /* c = C(n-k,k) for current k */
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = c * c * c;                 /* C(n-k,k)^3 */
        term = term * (seq_t)n / (seq_t)(n - k);/* multiply by n/(n-k) */
        sum += term;

        if (k == maxk) break;

        seq_t num1 = (seq_t)(n - 2 * k);
        seq_t num2 = (seq_t)(n - 2 * k - 1);
        seq_t denom1 = (seq_t)(n - k);
        seq_t denom2 = (seq_t)(k + 1);

        c = c * num1 * num2 / (denom1 * denom2);/* next C(n-(k+1),k+1) */
    }

    return sum;
}
