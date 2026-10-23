/*
 * A008797  Molien series for group [2,4]+ = 224.
 * https://oeis.org/A008797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2 + 3*n + 11 + 5*(n+1)*(-1)^n)/16)
 * Range:     u8 n <= 11, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;
    seq_t num;
    if ((n & 1) == 0) /* even n */
        num = nn * nn + 3 * nn + 11 + 5 * n1;
    else              /* odd n */
        num = nn * nn + 3 * nn + 11 - 5 * n1;
    return num / 16;
}
