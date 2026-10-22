/*
 * A005868  Molien series for 3-dimensional representation of Z2 X (double
 *          cover of A6), u.g.g.r. # 27 of Shephard and Todd.
 * https://oeis.org/A005868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n^2 + (1+15*(-1)^n)*n + 80)/80)
 * Range:     u8 n <= 7, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n_squared = nn * nn;
    seq_t numerator;
    
    if (n % 2 == 0) {
        numerator = n_squared + 16 * nn + 80;
    } else {
        seq_t n_minus_7;
        if (nn >= 7) {
            n_minus_7 = nn - 7;
        } else {
            n_minus_7 = 7 - nn;
        }
        numerator = n_minus_7 * n_minus_7 + 31;
    }
    
    return numerator / 80;
}
