/*
 * A003485  Hurwitz-Radon function at powers of 2.
 * https://oeis.org/A003485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = ceiling((n+1)/4) + ceiling((n)/4) + 2*ceiling((n-1)/4) +
 *            4*ceiling((n-2)/4)
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long n_ll = (long long)n;
    
    long long c1 = (n_ll + 4) / 4;
    long long c2 = (n_ll + 3) / 4;
    long long c3, c4;
    
    if (n_ll >= 1) {
        c3 = (n_ll + 2) / 4;
    } else {
        c3 = (n_ll - 1) / 4;
    }
    
    if (n_ll >= 2) {
        c4 = (n_ll + 1) / 4;
    } else {
        c4 = (n_ll - 2) / 4;
    }
    
    return (seq_t)(c1 + c2 + 2*c3 + 4*c4);
}
