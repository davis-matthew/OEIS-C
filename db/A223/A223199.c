/*
 * A223199  Rolling cube footprints: number of n X 5 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal or vertical neighbor moves across a corresponding cube
 *          edge.
 * https://oeis.org/A223199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 512*a(n-1) - 66560*a(n-2) + 3014656*a(n-3) -
 *            50331648*a(n-4) + 268435456*a(n-5); a(1)=256, a(2)=82944,
 *            a(3)=27951104, a(4)=9515827200, a(5)=3249109204992
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset – OEIS uses a(1) as the first term */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 – harmless */

    static const seq_t init[5] = {
        (seq_t)256ULL,
        (seq_t)82944ULL,
        (seq_t)27951104ULL,
        (seq_t)9515827200ULL,
        (seq_t)3249109204992ULL
    };

    if (n <= 5)
        return init[n - 1];

    /* Sliding window for the last five terms */
    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];
    seq_t an;

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) = 512*a5 + 3014656*a3 + 268435456*a1
                        - 66560*a4 - 50331648*a2    */
        an = (seq_t)512ULL * a5;
        an += (seq_t)3014656ULL * a3;
        an += (seq_t)268435456ULL * a1;
        an -= (seq_t)66560ULL * a4;
        an -= (seq_t)50331648ULL * a2;

        /* Shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
