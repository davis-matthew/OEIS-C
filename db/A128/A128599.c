/*
 * A128599  a(n) = the numerator of the continued fraction
 *          [[n/1];[n/2],[n/3],..,[n/n]] = the numerator of
 *          [[n/n];[n/(n-1)],[n/(n-2)],..,[n/1]], where [x] is floor(x).
 * https://oeis.org/A128599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of the continued fraction
 *            [[n/1];[n/2],[n/3],..., [n/n]]
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* below offset – undefined, but safe */
    seq_t prev = (seq_t)1;              /* p_{-1} */
    seq_t curr = (seq_t)n;              /* p_{0} = floor(n/1) = n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = (seq_t)n / (seq_t)i;   /* floor(n / i) */
        seq_t next = term * curr + prev;    /* p_i = a_i * p_{i-1} + p_{i-2} */
        prev = curr;
        curr = next;
    }
    return curr;
}
