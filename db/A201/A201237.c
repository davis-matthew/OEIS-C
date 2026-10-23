/*
 * A201237  Number of ways to place 3 non-attacking wazirs on an n X n
 *          toroidal board.
 * https://oeis.org/A201237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n^4-15*n^2+62)/6 for n >= 4 (a(1)=0, a(2)=0, a(3)=6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;
    if (n == 3)
        return 6;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n4  = n2 * n2;
    seq_t inside = n4 - (seq_t)15 * n2 + (seq_t)62;

    seq_t num = n2;          /* part that will be divided by 6 */

    /* divide by 2 */
    if (num % 2 == 0)
        num /= 2;
    else
        inside /= 2;

    /* divide by 3 */
    if (num % 3 == 0)
        num /= 3;
    else
        inside /= 3;

    return num * inside;
}
