/*
 * A006676  Number of paths through an array.
 * https://oeis.org/A006676
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = - Sum_{k=3..n} binomial(k, 3) * s(n+1, n+1-k) * n^(n-k),
 *            which simplifies to a(n) = n! * e_2((n-1)/1, (n-2)/2, ...,
 *            1/(n-1)), i.e. a(n) = n * [z^2] Product_{m=1..n-1} (m + (n-m)*z)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c0 = 1, c1 = 0, c2 = 0;
    idx_t m;
    if (n < 3) return 0;
    for (m = 1; m < n; m++) {
        seq_t sm = (seq_t)m;
        seq_t sr = (seq_t)(n - m);
        c2 = c2 * sm + c1 * sr;
        c1 = c1 * sm + c0 * sr;
        c0 = c0 * sm;
    }
    return (seq_t)n * c2;
}
