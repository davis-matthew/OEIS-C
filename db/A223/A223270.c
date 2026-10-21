/*
 * A223270  Rolling cube footprints: number of 2 X n 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves across a
 *          corresponding cube edge.
 * https://oeis.org/A223270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 32*a(n-2) with a(1)=6, a(2)=48
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; 0 is unused */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)48;

    seq_t prev2 = (seq_t)6;                 /* a(1) */
    seq_t prev1 = (seq_t)48;                /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (prev1 * (seq_t)8) + (prev2 * (seq_t)32);
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
