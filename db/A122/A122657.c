/*
 * A122657  a(n) = if n mod 2 = 1 then (n^2-1)*n^3/4 else n^5/4.
 * https://oeis.org/A122657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n mod 2 = 1 then (n^2-1)*n^3/4 else n^5/4
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 110, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    if ((n & 1ULL) == 0)   /* even n: a(n) = n^5 / 4 */
    {
        seq_t t = N * N * N * N * N;   /* n^5   */
        t /= 4;
        return t;
    }
    else                   /* odd n : a(n) = (n^2-1)*n^3 / 4 */
    {
        seq_t t = N * N;          /* n^2       */
        t -= 1;                   /* n^2 - 1   */
        t *= N * N * N;           /* * n^3     */
        t /= 4;
        return t;
    }
}
