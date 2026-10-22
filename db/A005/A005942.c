/*
 * A005942  a(2n) = a(n) + a(n+1), a(2n+1) = 2a(n+1), if n >= 2.
 * https://oeis.org/A005942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(2n) = a(n) + a(n+1), a(2n+1) = 2a(n+1), for n >= 2; initial:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=6
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 4;
    if (n == 3) return 6;
    
    if (n % 2 == 0) {
        idx_t k = n / 2;
        return a(k) + a(k + 1);
    } else {
        idx_t k = (n - 1) / 2;
        return 2 * a(k + 1);
    }
}
