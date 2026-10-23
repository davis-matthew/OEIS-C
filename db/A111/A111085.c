/*
 * A111085  Number of semi-magic 3-dimensional hypercubes with 27 entries and
 *          magic sum n.
 * https://oeis.org/A111085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*(-1)^n/256 + 513*n^3/160 + 3653*n^2/1120 +
 *            27*(-1)^n*n/128 + 1341*n^4/640 + 297*n^5/320 + 87*n^6/320 +
 *            27*n^7/560 + 9*n^8/2240 + 175/256 + 9087*n/4480 (integer for all
 *            n ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Common denominator = lcm(256,160,1120,128,640,320,560,2240,256,4480) = 8960
        The result is (sum of scaled numerators) / 8960. */
    int sign = (n & 1) ? -1 : 1;           /* (-1)^n */
    __int128   s  = 0;                    /* accumulates scaled numerators (signed) */

    /* pre‑compute powers of n */
    __int128 n128 = n;
    __int128 n2   = n128 * n128;          /* n^2 */
    __int128 n3   = n2   * n128;          /* n^3 */
    __int128 n4   = n3   * n128;          /* n^4 */
    __int128 n5   = n4   * n128;          /* n^5 */
    __int128 n6   = n5   * n128;          /* n^6 */
    __int128 n7   = n6   * n128;          /* n^7 */
    __int128 n8   = n7   * n128;          /* n^8 */

    /* term  1: 81*(-1)^n/256  → scale 35 = 8960/256 */
    s += (__int128)sign * 81 * 35;

    /* term  2: 513*n^3/160   → scale 56 = 8960/160 */
    s += (__int128)513 * n3 * 56;

    /* term  3: 3653*n^2/1120 → scale 8  = 8960/1120 */
    s += (__int128)3653 * n2 * 8;

    /* term  4: 27*(-1)^n*n/128 → scale 70 = 8960/128 */
    s += (__int128)sign * 27 * n128 * 70;

    /* term  5: 1341*n^4/640  → scale 14 = 8960/640  */
    s += (__int128)1341 * n4 * 14;

    /* term  6: 297*n^5/320   → scale 28 = 8960/320  */
    s += (__int128)297 * n5 * 28;

    /* term  7: 87*n^6/320    → scale 28 = 8960/320  */
    s += (__int128)87 * n6 * 28;

    /* term  8: 27*n^7/560    → scale 16 = 8960/560  */
    s += (__int128)27 * n7 * 16;

    /* term  9:  9*n^8/2240   → scale 4  = 8960/2240 */
    s += (__int128)9 * n8 * 4;

    /* term 10: 175/256      → scale 35 = 8960/256 */
    s += (__int128)175 * 35;

    /* term 11: 9087*n/4480  → scale 2  = 8960/4480 */
    s += (__int128)9087 * n128 * 2;

    /* final result is integer division by 8960 */
    seq_t result = (seq_t)(s / 8960);
    return result;
}
