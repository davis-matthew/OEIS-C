/*
 * A004128  a(n) = Sum_{k=1..n} floor(3*n/3^k).
 * https://oeis.org/A004128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..inf} floor(n/3^k), or a(n) = n + a(floor(n/3))
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t temp = (seq_t)n;
    while (temp > 0) {
        result += temp;
        temp /= 3;
    }
    return result;
}
