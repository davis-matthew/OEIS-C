/*
 * A116755  Number of permutations of length n which avoid the patterns 1234,
 *          2431, 3412.
 * https://oeis.org/A116755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^6 - 45*n^5 + 175*n^4 - 315*n^3 + 538*n^2 - 360*n +
 *            720)/720
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Safety for non‑positive indices */
    if (n == 0)
        return 0;

    /* Use a signed 128‑bit integer for intermediate arithmetic */
    __int128 N   = n;
    __int128 poly = 7;          /* 7 */
    poly = poly * N - 45;       /* 7*n - 45          */
    poly = poly * N + 175;      /* (7*n - 45)*n + 175*/
    poly = poly * N - 315;      /* ... - 315*n        */
    poly = poly * N + 538;      /* ... + 538*n        */
    poly = poly * N - 360;      /* ... - 360*n        */
    poly = poly * N + 720;      /* ... + 720*n        */

    /* Final division gives the required value */
    seq_t result = (seq_t)(poly / 720);
    return result;
}
