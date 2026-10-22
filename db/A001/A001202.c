/*
 * A001202  a(1)=0, a(2n) = a(n)+1, a(2n+1) = 10*a(n+1).
 * https://oeis.org/A001202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2n)=a(n)+1, a(2n+1)=10*a(n+1)
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 1024, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n % 2 == 0) {
        return a(n / 2) + 1;
    } else {
        return 10 * a((n + 1) / 2);
    }
}
