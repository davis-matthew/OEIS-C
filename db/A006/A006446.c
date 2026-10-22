/*
 * A006446  Numbers k such that floor(sqrt(k)) divides k.
 * https://oeis.org/A006446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(3*k-2) = k^2, a(3*k-1) = k*(k+1), a(3*k) = k*(k+2)
 * Range:     u8 n <= 45, u16 n <= 765, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t k;
    
    idx_t rem = n % 3;
    
    if (rem == 1) {
        k = (m + 2) / 3;
        return k * k;
    } else if (rem == 2) {
        k = (m + 1) / 3;
        return k * (k + 1);
    } else {
        k = m / 3;
        return k * (k + 2);
    }
}
