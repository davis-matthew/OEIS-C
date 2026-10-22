/*
 * A007612  a(n+1) = a(n) + digital root (A010888) of a(n).
 * https://oeis.org/A007612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = 27*floor((n-1)/6) + c where c ∈ {1,2,4,8,16,23} based on
 *            (n-1) mod 6
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t coefficients[] = {1, 2, 4, 8, 16, 23};
    return (seq_t)27 * (seq_t)((n - 1) / 6) + coefficients[(n - 1) % 6];
}
