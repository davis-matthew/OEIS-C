/*
 * A002625  Expansion of 1/((1-x)^3*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A002625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(135*(-1)^n + 6*n^4 + 144*n^3 + 1256*n^2 +
 *            4744*n + 6785)/8640+1/2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t nn4 = nn2 * nn2;
    
    seq_t poly = (seq_t)6 * nn4 + (seq_t)144 * nn3 + (seq_t)1256 * nn2 
               + (seq_t)4744 * nn + (seq_t)6785;
    
    if (n % 2 == 0) {
        poly += (seq_t)135;
    } else {
        poly -= (seq_t)135;
    }
    
    seq_t numerator = (nn + (seq_t)1) * poly + (seq_t)4320;
    
    return numerator / (seq_t)8640;
}
