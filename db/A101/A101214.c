/*
 * A101214  a(n) = n * (n+1)^2 * (n+2)^3 * (n+3)^4.
 * https://oeis.org/A101214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n+1)^2 * (n+2)^3 * (n+3)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 82, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t n1  = nn + 1;
    seq_t n2  = nn + 2;
    seq_t n3  = nn + 3;

    seq_t res = nn;            /* n */

    /* (n+1)^2 */
    res *= n1;
    res *= n1;

    /* (n+2)^3 */
    res *= n2;
    res *= n2;
    res *= n2;

    /* (n+3)^4 */
    res *= n3;
    res *= n3;
    res *= n3;
    res *= n3;

    return res;
}
