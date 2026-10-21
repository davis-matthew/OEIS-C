/*
 * A054898  a(n) = Sum_{k>0} floor(n/9^k).
 * https://oeis.org/A054898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>0} floor(n/9^k)
 * Range:     u8 n <= 2060, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t t = n;
    while (t > 0) {
        t /= 9;
        sum += (seq_t)t;
    }
    return sum;
}
