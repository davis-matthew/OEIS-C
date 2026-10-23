/*
 * A155753  a(n) = (n^3 - n + 9)/3.
 * https://oeis.org/A155753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n + 9)/3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2343, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at n = 1 */
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn * nn - nn + (seq_t)9;  /* (n^3 - n + 9) */
    return term / (seq_t)3;                    /* division by 3 */
}
