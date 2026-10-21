/*
 * A223333  Rolling cube footprints: number of 4 X n 0..7 arrays starting with
 *          0 where 0..7 label vertices of a cube and every array movement to
 *          a horizontal or antidiagonal neighbor moves along a corresponding
 *          cube edge.
 * https://oeis.org/A223333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1) - 402*a(n-2) + 1064*a(n-3) - 789*a(n-4) for
 *            n>7. Initial values: a(1)=512, a(2)=2187, a(3)=83349,
 *            a(4)=3176523, a(5)=121264857, a(6)=4630596579, a(7)=176834343105
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 1 .. 7.  (0‑based array indices) */
    static const seq_t base[7] = {
        (seq_t)512,
        (seq_t)2187,
        (seq_t)83349,
        (seq_t)3176523,
        (seq_t)121264857,
        (seq_t)4630596579ULL,
        (seq_t)176834343105ULL
    };

    if (n == 0)
        return (seq_t)0;              /* undefined for n < 1 */

    if (n <= 7)
        return base[n - 1];           /* return pre‑computed value */

    /* Iterate using the recurrence.  The variables a1 .. a7 hold
       the seven most recent terms, with a7 being a(n‑1). */
    seq_t a1 = base[0];
    seq_t a2 = base[1];
    seq_t a3 = base[2];
    seq_t a4 = base[3];
    seq_t a5 = base[4];
    seq_t a6 = base[5];
    seq_t a7 = base[6];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = 48 * a7            /* a(n-1) */
                   - 402 * a6           /* a(n-2) */
                   + 1064 * a5          /* a(n-3) */
                   - 789  * a4;         /* a(n-4) */

        /* Shift the window forward. */
        a1 = a2; a2 = a3; a3 = a4;
        a4 = a5; a5 = a6; a6 = a7;
        a7 = next;
    }
    return a7;
}
