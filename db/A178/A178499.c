/*
 * A178499  Number of ways to place 6 nonattacking knights on an n X n board.
 * https://oeis.org/A178499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = n^12/720-(3*n^10)/16+n^9/2+(1553*n^8)/144-(163*n^7)/3-
 *            * (4493*n^6)/16+(4721*n^5)/2+(578777*n^4)/360-(143156*n^3)/3+ *
 *            (124917*n^2)/2+374990*n-899982 for n >= 10
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 11, u64 n <= 69, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 10 the polynomial does not give correct values,
       so use a small table for those indices. */
    if (n < 10) {
        static const seq_t table[9] = {
            0ULL,                /* a(1) */
            0ULL,                /* a(2) */
            0ULL,                /* a(3) */
            170ULL,              /* a(4) */
            13384ULL,            /* a(5) */
            257318ULL,           /* a(6) */
            2774728ULL,          /* a(7) */
            20202298ULL,         /* a(8) */
            110018552ULL         /* a(9) */
        };
        return table[n - 1];
    }

    /* Use a 128‑bit integer for all intermediate calculations
       to avoid overflow on 64‑bit builds.  This works for all
       n that fit in seq_t (the test harness limits n for 64‑bit
       builds to values where a(n) fits into 64 bits). */
    __int128 nn = n;
    __int128 pow[13];
    pow[0] = 1;
    for (int i = 1; i <= 12; ++i)
        pow[i] = pow[i - 1] * nn;

    __int128 num = pow[12];
    num -= (__int128)135 * pow[10];
    num += (__int128)360 * pow[9];
    num += (__int128)7765 * pow[8];
    num -= (__int128)39120 * pow[7];
    num -= (__int128)202185 * pow[6];
    num += (__int128)1699560 * pow[5];   /* corrected coefficient */
    num += (__int128)1157554 * pow[4];
    num -= (__int128)34357440 * pow[3];
    num += (__int128)44970120 * pow[2];
    num += (__int128)269992800 * pow[1];
    num -= (__int128)647987040;

    /* The division is exact for all n >= 10. */
    return (seq_t)(num / 720);
}
