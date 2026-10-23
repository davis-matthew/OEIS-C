/*
 * A174021  Number of symmetry classes of reduced 3 X 3 magilatin squares with
 *          magic sum n.
 * https://oeis.org/A174021
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n^2-4*n+36)/48 + (n mod 2)*(n^2-9*n+17)/16 +
 *            ((2*n^2+1) mod 3)*(n-3)/3 + ((n^3+n^2+n+2) mod 4)*n/8 - 3/2*[(n
 *            mod 4)=0] - [(n mod 5)=0] - 2*[(n mod 6)=0] - [(n mod 7)=0] -
 *            [(n mod 8)=0]
 * Range:     u8 n <= 24, u16 n <= 148, u32 n <= 5908, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* Work in signed 128‑bit to handle negative intermediate terms safely */
    __int128 nn = n;
    __int128 n_sq = nn * nn;

    /* Modular reductions needed for the indicator terms */
    int mod2 = n % 2;

    int r3 = n % 3;
    int val3 = (2 * ((r3 * r3) % 3) + 1) % 3;       /* (2*n^2+1) mod 3 */

    int r4 = n % 4;
    int r4_sq = (r4 * r4) % 4;
    int val4 = (r4_sq * r4 % 4 + r4_sq + r4 + 2) % 4; /* (n^3+n^2+n+2) mod 4 */

    int c4 = (n % 4 == 0);
    int c5 = (n % 5 == 0);
    int c6 = (n % 6 == 0);
    int c7 = (n % 7 == 0);
    int c8 = (n % 8 == 0);

    __int128 N = 0;

    /* term1 */
    N += (nn - 2) * (n_sq - (__int128)4 * nn + 36);

    /* term2 */
    N += (__int128)3 * mod2 * (n_sq - (__int128)9 * nn + 17);

    /* term3 */
    N += (__int128)16 * val3 * (nn - 3);

    /* term4 */
    N += (__int128)6 * val4 * nn;

    /* subtraction of the indicator contributions */
    N -= (__int128)72 * c4;  /* -3/2 * 48 = -72 */
    N -= (__int128)48 * c5;  /* -1   * 48 = -48 */
    N -= (__int128)96 * c6;  /* -2   * 48 = -96 */
    N -= (__int128)48 * c7;  /* -1   * 48 = -48 */
    N -= (__int128)48 * c8;  /* -1   * 48 = -48 */

    return (seq_t)(N / 48);
}
