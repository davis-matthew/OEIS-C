/*
 * A000788  Total number of 1's in binary expansions of 0, ..., n.
 * https://oeis.org/A000788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, base, easy
 * Formula:   a(n) = Sum_{k=0..n} popcount(k), via recurrence: a(0) = 0, a(2m)
 *            = a(m) + a(m-1) + m, a(2m+1) = 2*a(m) + m + 1
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    if (n & 1) {
        idx_t m = n >> 1;
        return 2 * a(m) + (seq_t)m + 1;
    } else {
        idx_t m = n >> 1;
        return a(m) + a(m - 1) + (seq_t)m;
    }
}
