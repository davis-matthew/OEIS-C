/*
 * A081600  Let n = 10x + y where 0 <= y <= 9, x >= 0. Then a(n) = 9x+y.
 * https://oeis.org/A081600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n/10)
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n - (seq_t)(n / 10);
}
