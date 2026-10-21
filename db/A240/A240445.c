/*
 * A240445  Numbers of ways to place five indistinguishable points on an n X n
 *          square grid so that no four of them are vertices of a square of
 *          any orientation.
 * https://oeis.org/A240445
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 - 10*n^8 + 25*n^6 - 16*n^2)/120
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 136, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Compute using 128‑bit intermediate to avoid overflow for all
       values that fit in seq_t.  The result fits in seq_t when it
       is representable. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;          /* n^2 */
    unsigned __int128 n4  = n2 * n2;        /* n^4 */
    unsigned __int128 n6  = n4 * n2;        /* n^6 */
    unsigned __int128 n8  = n4 * n4;        /* n^8 */
    unsigned __int128 n10 = n8 * n2;        /* n^10 */

    unsigned __int128 num =
          n10
        - (unsigned __int128)10 * n8
        + (unsigned __int128)25 * n6
        - (unsigned __int128)16 * n2;

    unsigned __int128 res = num / 120;

    return (seq_t)res;
}
