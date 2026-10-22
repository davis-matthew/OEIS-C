/*
 * A002413  Heptagonal (or 7-gonal) pyramidal numbers: a(n) =
 *          n*(n+1)*(5*n-2)/6.
 * https://oeis.org/A002413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(5*n-2)/6
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn1 = nn + 1;
    seq_t term = nn * 5 - 2;
    
    // Divide by 3
    if (nn % 3 == 0) {
        nn = nn / 3;
    } else if (nn1 % 3 == 0) {
        nn1 = nn1 / 3;
    } else {
        term = term / 3;
    }
    
    // Divide by 2 (one of nn or nn1 is always even)
    if (nn % 2 == 0) {
        nn = nn / 2;
    } else {
        nn1 = nn1 / 2;
    }
    
    return nn * nn1 * term;
}
