/*
 * A028294  a(n) = n^5 - (65/6)*n^4 + (173/6)*n^3 + (148/3)*n^2 - (862/3)*n +
 *          265.
 * https://oeis.org/A028294
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A028294
 *  a(n) = n^5 - (65/6)*n^4 + (173/6)*n^3 + (148/3)*n^2 - (862/3)*n + 265
 *
 *  The implementation evaluates the integer polynomial safely using a 128‑bit
 *  (signed) intermediate to avoid overflow for n up to 10 000.  Results that
 *  fit into seq_t are returned exactly; larger results truncate as required
 *  by the build’s seq_t type.
 */
seq_t a(idx_t n)
{
    if (n < 4) /* undefined for the official offset; return 0 */
        return 0;

    /* use signed __int128 for intermediate calculation */
    __int128 N   = (__int128)n;
    __int128 num =
        6 * N * N * N * N * N       /* 6 n^5   */
        - 65 * N * N * N * N       /* -65 n^4 */
        + 173 * N * N * N          /* +173 n^3*/
        + 296 * N * N              /* +296 n^2*/
        - 1724 * N                 /* -1724 n */
        + 1590;                    /* +1590   */

    __int128 result = num / 6;     /* exact division */
    return (seq_t)result;
}
