/*
 * A054622  Number of ways to color vertices of an octagon using <= n colors,
 *          allowing only rotations.
 * https://oeis.org/A054622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 + n^4 + 2*n^2 + 4*n)/8 (OEIS A054622)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 331, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Use a 128‑bit intermediate to avoid overflow on 64‑bit builds
       while staying compatible with an arbitrary‑precision seq_t. */
    __int128 N   = n;                      /* n as 128‑bit integer */
    __int128 n2  = N * N;                  /* n^2 */
    __int128 n4  = n2 * n2;                /* n^4 */
    __int128 n8  = n4 * n4;                /* n^8 */

    __int128 result = (n8 + n4 + 2 * n2 + 4 * N) / 8;
    return (seq_t)result;                  /* cast to seq_t (uint64_t or arbitrary) */
}
