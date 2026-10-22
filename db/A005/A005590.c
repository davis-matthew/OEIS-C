/*
 * A005590  a(0) = 0, a(1) = 1, a(2n) = a(n), a(2n+1) = a(n+1) - a(n).
 * https://oeis.org/A005590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy, look
 * Formula:   a(0) = 0, a(1) = 1, a(2n) = a(n), a(2n+1) = a(n+1) - a(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    if (n & 1) {  // n is odd: a(2k+1) = a(k+1) - a(k)
        idx_t k = n >> 1;
        return a(k + 1) - a(k);
    } else {  // n is even: a(2k) = a(k)
        return a(n >> 1);
    }
}
