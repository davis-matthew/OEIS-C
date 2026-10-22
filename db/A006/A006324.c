/*
 * A006324  a(n) = n*(n + 1)*(2*n^2 + 2*n - 1)/6.
 * https://oeis.org/A006324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 1)*(2*n^2 + 2*n - 1)/6
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 335, u64 n <= 8899,
 *            u128 n <= 8899, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t m;
    
    if (nn % 2 == 0) {
        m = (nn / 2) * (nn + 1);
    } else {
        m = nn * ((nn + 1) / 2);
    }
    
    return m * (4 * m - 1) / 3;
}
