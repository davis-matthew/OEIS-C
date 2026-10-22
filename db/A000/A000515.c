/*
 * A000515  a(n) = (2n)!(2n+1)!/n!^4, or equally (2n+1)*binomial(2n,n)^2.
 * https://oeis.org/A000515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2n+1)*binomial(2n,n)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    /* c = binomial(n+k, k), built multiplicatively; each step divides exactly */
    for (k = 1; k <= n; k++) {
        c = c * ((seq_t)n + (seq_t)k) / (seq_t)k;
    }
    return (2 * (seq_t)n + 1) * c * c;
}
