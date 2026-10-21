/*
 * A051836  a(n) = n*(n+1)*(n+2)*(n+3)*(3*n+2)/120.
 * https://oeis.org/A051836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(3*n+2)/120
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    __int128 nn = n;
    __int128 val = nn * (nn + 1) * (nn + 2) * (nn + 3) * (3 * nn + 2);
    val /= 120;
    return (seq_t)val;
}
