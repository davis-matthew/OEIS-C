/*
 * A025607  Number of n-move rook paths on 8 X 8 board from given corner to
 *          same corner.
 * https://oeis.org/A025607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 18*a(n-1) - 44*a(n-2) - 168*a(n-3) Exact values: a(0) =
 *            1, a(1) = 0, a(2) = 14
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)14;

    /* Use a simple linear recurrence: keep the last three terms */
    seq_t a0 = 1;        /* a(0) */
    seq_t a1 = 0;        /* a(1) */
    seq_t a2 = 14;       /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)18 * a2;
        an -= (seq_t)44 * a1;
        an -= (seq_t)168 * a0;
        /* Roll the window forward */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
