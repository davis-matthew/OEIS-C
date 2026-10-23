/*
 * A187292  Number of 7-step one or two space at a time rook's tours on an n X
 *          n board summed over all starting positions.
 * https://oeis.org/A187292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>14 Initial values:
 *            a(1)=0, a(2)=0, a(3)=2304, a(4)=47648, a(5)=228512, a(6)=616752,
 *            a(7)=1241936, a(8)=2102944, a(9)=3192912, a(10)=4505424,
 *            a(11)=6037932, a(12)=7789392, a(13)=9759616, a(14)=11948560
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed initial values up to n = 14 */
    static const seq_t init[15] = {
        0ULL,       /* 0 placeholder for index 0 */
        0ULL,       /* a(1) */
        0ULL,       /* a(2) */
        2304ULL,    /* a(3) */
        47648ULL,   /* a(4) */
        228512ULL,  /* a(5) */
        616752ULL,  /* a(6) */
        1241936ULL, /* a(7) */
        2102944ULL, /* a(8) */
        3192912ULL, /* a(9) */
        4505424ULL, /* a(10) */
        6037932ULL, /* a(11) */
        7789392ULL, /* a(12) */
        9759616ULL, /* a(13) */
        11948560ULL /* a(14) */
    };

    if (n <= 14)
        return init[(idx_t)n];

    /* Compute using the recurrence a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) */
    seq_t a1 = init[12]; /* a(n-3)  */
    seq_t a2 = init[13]; /* a(n-2)  */
    seq_t a3 = init[14]; /* a(n-1)  */

    for (idx_t i = 15; i <= n; ++i) {
        seq_t next = (seq_t)3 * a3 - (seq_t)3 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
