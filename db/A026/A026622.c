/*
 * A026622  a(n) = Sum_{k=0..n} A026615(n, k).
 * https://oeis.org/A026622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2, a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    seq_t val = (seq_t)5;
    for (idx_t i = 3; i <= n; ++i) {
        val = val * (seq_t)2 + (seq_t)2;
    }
    return val;
}
