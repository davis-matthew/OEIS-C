/*
 * A061313  Minimal number of steps to get from 1 to n by (a) subtracting 1 or
 *          (b) multiplying by 2.
 * https://oeis.org/A061313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0; a(2n)=a(n)+1; a(2n+1)=a(n+1)+2;
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if ((n & 1) == 0) {          /* n is even: a(2k) = a(k) + 1 */
        return a(n >> 1) + 1;
    } else {                     /* n is odd: a(2k+1) = a(k+1) + 2 */
        return a((n >> 1) + 1) + 2;
    }
}
