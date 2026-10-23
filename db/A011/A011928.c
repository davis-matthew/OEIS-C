/*
 * A011928  a(n) = floor(n(n-1)(n-2)(n-3)/18).
 * https://oeis.org/A011928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/18)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return 0;
    seq_t val = (seq_t)n;
    val *= (n - 1);
    val *= (n - 2);
    val *= (n - 3);
    val /= 18;
    return val;
}
