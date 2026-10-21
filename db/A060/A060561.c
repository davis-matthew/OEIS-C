/*
 * A060561  Number of ways to color vertices of a 9-gon using <= n colors,
 *          allowing rotations and reflections.
 * https://oeis.org/A060561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^9 + 9*n^5 + 2*n^3 + 6*n)/18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 190, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate values
       to avoid overflow even when seq_t is 64‑bit. */
    unsigned __int128 N  = n;
    unsigned __int128 n2 = N * N;          /* n^2 */
    unsigned __int128 n3 = n2 * N;         /* n^3 */
    unsigned __int128 n4 = n2 * n2;        /* n^4 */
    unsigned __int128 n5 = n4 * N;         /* n^5 */
    unsigned __int128 n9 = n5 * n4;        /* n^9 */

    unsigned __int128 sum = n9
                          + (unsigned __int128)9 * n5
                          + (unsigned __int128)2 * n3
                          + (unsigned __int128)6 * N;

    unsigned __int128 res128 = sum / 18;
    return (seq_t)res128;
}
