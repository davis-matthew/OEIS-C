/*
 * A006468  Number of rooted planar maps with 4 faces and n vertices and no
 *          isthmuses.
 * https://oeis.org/A006468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(2*n^3 + 33*n^2 + 142*n + 123) / 360
 * Range:     u8 none, u16 n <= 12, u32 n <= 54, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    
    seq_t temp;
    if (nn % 2 == 0) {
        temp = (nn / 2) * (nn + 1) * (nn + 2);
    } else {
        temp = nn * ((nn + 1) / 2) * (nn + 2);
    }
    seq_t binom_coef = temp / 3;
    
    seq_t poly = 2*nn*nn*nn + 33*nn*nn + 142*nn + 123;
    
    return binom_coef * poly / 60;
}
