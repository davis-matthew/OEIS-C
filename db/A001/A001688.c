/*
 * A001688  4th forward differences of factorial numbers A000142.
 * https://oeis.org/A001688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n^4 + 6*n^3 + 17*n^2 + 20*n + 9)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    
    seq_t fact = 1;
    for (seq_t i = 2; i <= sn; i++) {
        fact *= i;
    }
    
    seq_t poly = ((((sn + 6) * sn + 17) * sn + 20) * sn + 9);
    
    return fact * poly;
}
