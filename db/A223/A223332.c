/*
 * A223332  Rolling cube footprints: number of 3 X n 0..7 arrays starting with
 *          0 where 0..7 label vertices of a cube and every array movement to
 *          a horizontal or antidiagonal neighbor moves along a corresponding
 *          cube edge.
 * https://oeis.org/A223332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 27*a(n-2) for n > 4 Initial values: a(1)=64,
 *            a(2)=243, a(3)=3969, a(4)=64827
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* safety for off‑by‑one consumers */

    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)243;
    if (n == 3) return (seq_t)3969;
    if (n == 4) return (seq_t)64827;

    seq_t prev2 = (seq_t)3969;   /* a(n-2) when i = 5  */
    seq_t prev1 = (seq_t)64827;  /* a(n-1) when i = 5  */
    seq_t curr  = 0;

    for (idx_t i = 5; i <= n; ++i) {
        curr = (seq_t)18 * prev1 - (seq_t)27 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
