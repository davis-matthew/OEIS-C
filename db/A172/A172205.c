/*
 * A172205  Number of ways to place 6 nonattacking kings on a 6 X n board.
 * https://oeis.org/A172205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*(162*n^6-3240*n^5+29160*n^4-151830*n^3+483798*n^2-895085*n+749335)/5
 *            for n≥5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 23, u64 n <= 814, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first few terms directly to avoid the polynomial at small n */
    if (n == 1 || n == 2)
        return 0;
    if (n == 3)
        return 16;
    if (n == 4)
        return 408;

    /* use 128‑bit arithmetic for intermediate calculations */
    __int128 N  = (__int128)n;
    __int128 N2 = N * N;
    __int128 N3 = N2 * N;
    __int128 N4 = N3 * N;
    __int128 N5 = N4 * N;
    __int128 N6 = N5 * N;

    __int128 numerator = 2 *
        ((__int128)162   * N6
        - (__int128)3240 * N5
        + (__int128)29160 * N4
        - (__int128)151830 * N3
        + (__int128)483798 * N2
        - (__int128)895085 * N
        + (__int128)749335);

    /* the expression is guaranteed divisible by 5 for n ≥ 5 */
    return (seq_t)(numerator / 5);
}
