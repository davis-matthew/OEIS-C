/*
 * A071042  Number of 0's in n-th row of triangle in A070886.
 * https://oeis.org/A071042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1 a(2n) = a(n) + 2n a(2n+1) = 2a(n) + 1 (the
 *            recurrence follows from the OEIS definition)
 * Range:     u8 n <= 129, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    if ((n & 1) == 0) {                // n is even: n = 2m
        return a(n >> 1) + (seq_t)n;   // a(n) = a(m) + 2m = a(m) + n
    } else {                           // n is odd: n = 2m + 1
        return (seq_t)2 * a(n >> 1) + 1;
    }
}
