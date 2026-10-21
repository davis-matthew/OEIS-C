/*
 * A238108  a(n) = (n - 1)*(n - 2)*(5*n^4 + 3*n^3 + 34*n^2 - 264*n + 180)/360.
 * https://oeis.org/A238108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*(n - 2)*(5*n^4 + 3*n^3 + 34*n^2 - 264*n +
 *            180)/360
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1 || n == 2)
        return 0;
    unsigned __int128 nn = n;
    unsigned __int128 n1 = nn - 1;
    unsigned __int128 n2 = nn - 2;
    unsigned __int128 n2_val = nn * nn;
    unsigned __int128 n3_val = n2_val * nn;
    unsigned __int128 n4_val = n3_val * nn;
    unsigned __int128 poly = 5 * n4_val + 3 * n3_val + 34 * n2_val - 264 * nn + 180;
    unsigned __int128 num = n1 * n2 * poly;
    unsigned __int128 res = num / (unsigned __int128)360;
    return (seq_t)res;
}
