/*
 * A206810  Sum_{0<j<n} (n^4-j^4).
 * https://oeis.org/A206810
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-10*n^3-15*n^4+24*n^5)/30
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 88, u64 n <= 7457,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative implementation by summing n^4 - j^4 for 0 < j < n */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t ns = (seq_t)n;

    /* n^2 and n^4 */
    seq_t n2 = ns * ns;
    seq_t n4 = n2 * n2;

    /* Sum j^4 for j = 1 .. n-1 */
    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j < n; ++j) {
        seq_t js = (seq_t)j;
        seq_t j2 = js * js;
        seq_t j4 = j2 * j2;
        sum += j4;
    }

    /* a(n) = (n-1)*n^4 - sum_{j=1}^{n-1} j^4 */
    seq_t result = (ns - (seq_t)1) * n4 - sum;
    return result;
}
