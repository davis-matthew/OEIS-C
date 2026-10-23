/*
 * A049779  a(n) = Sum_{k=1..floor(n/2)} T(n, 2k), array T as in A049777.
 * https://oeis.org/A049779
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k*floor(k/2)
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        sum += kk * (kk >> 1);
    }
    return sum;
}
