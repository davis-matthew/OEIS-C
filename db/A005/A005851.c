/*
 * A005851  The coding-theoretic function A(n,8,5).
 * https://oeis.org/A005851
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = floor(n*(n-1)/20) - correction[n mod 20]
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t correction[] = {3, 0, 2, 2, 3, 0, 0, 1, 1, 1, 2, 3, 3, 4, 5, 4, 6, 6, 6, 5};
    seq_t sn = (seq_t)n;
    seq_t base = sn * (sn - 1) / 20;
    seq_t corr = correction[n % 20];
    
    if ((n % 20 == 6) && (n >= 20)) {
        corr = 2;
    }
    
    return base - corr;
}
