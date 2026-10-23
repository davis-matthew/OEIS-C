/*
 * A190813  Positions of 0 in A189480.
 * https://oeis.org/A190813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - T((n-1) mod 18) where the periodic sequence T of
 *            period 18 is: {3,3,3,3,3, 6,6,6,6,6, 5,5,5,5, 4,4,4,4}
 * Range:     u8 n <= 65, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behavior for n=0 is irrelevant   */
        return 0;

    /* the pattern repeats every 18 terms of n-1 */
    static const unsigned char T[18] =
    {
        3,3,3,3,3,          /* indices 0‑4 */
        6,6,6,6,6,          /* indices 5‑9 */
        5,5,5,5,            /* indices 10‑13 */
        4,4,4,4             /* indices 14‑17 */
    };

    idx_t k = (n - 1) % 18;                       /* 0‑based index in the cycle   */
    seq_t temp = (seq_t)n * (seq_t)4;             /* 4*n, cast to seq_t first    */
    return temp - (seq_t)T[k];                    /* subtract the periodic term   */
}
