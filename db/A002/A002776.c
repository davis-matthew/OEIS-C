/*
 * A002776  Terms in certain determinants.
 * https://oeis.org/A002776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n^3 + n^2 + 2*n + 1)*n!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    
    /* Compute polynomial: n^3 + n^2 + 2*n + 1 */
    seq_t poly = nn*nn*nn + nn*nn + 2*nn + 1;
    
    /* Compute n! */
    seq_t fact = 1;
    for (seq_t i = 2; i <= nn; i++) {
        fact *= i;
    }
    
    return poly * fact;
}
