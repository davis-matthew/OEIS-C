/*
 * A092352  G.f.: (1+3*x^3)/((1-x)^2*(1-x^3)^2).
 * https://oeis.org/A092352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((⌊n/3⌋+1)*(4*⌊n/3⌋² + 3*⌊n/3⌋ + 4*⌊n/3⌋*(n mod 3) + 2*(n
 *            mod 3) + 2))/2
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)(n / 3);
    seq_t r = (seq_t)(n % 3);
    seq_t inside = 4 * N * N + 3 * N + 4 * N * r + 2 * r + 2;
    seq_t numerator = (N + 1) * inside;
    return numerator / 2;
}
