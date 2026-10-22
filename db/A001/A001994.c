/*
 * A001994  Expansion of 1/((1-x)*(1-x^2)^2*(1-x^3)*(1-x^4)*(1-x^5)).
 * https://oeis.org/A001994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((6*n^5 + 255*n^4 + 4040*n^3 + 29325*n^2 + 95085*n +
 *            172800 + 675*(-1)^n*(n^2 + 17*n))/172800)
 * Range:     u8 none, u16 none, u32 n <= 76, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;
    seq_t n5 = n4 * sn;
    
    seq_t result = 6 * n5 + 255 * n4 + 4040 * n3 + 29325 * n2 + 95085 * sn + 172800;
    
    seq_t alt = 675 * (n2 + 17 * sn);
    if (n & 1) {
        result = result - alt;
    } else {
        result = result + alt;
    }
    
    return result / 172800;
}
