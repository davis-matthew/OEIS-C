/*
 * A172261  Number of ways to place 8 nonattacking kings on an 8 X n board.
 * https://oeis.org/A172261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1048576 n^8 - 30277632 n^7 + 406210560 n^6 * -
 *            3319585920 n^5 + 18136811049 n^4 * - 68048382318 n^3 +
 *            171628664735 n^2 * - 266425935930 n + 194935658400) / 2520 * for
 *            n >= 7; for 1 <= n < 7 the values are given explicitly
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 124, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for the defined offset */
    if (n < 1)
        return 0;

    if (n < 7) {
        static const seq_t tbl[6] = {0ULL, 0ULL, 25ULL, 1847ULL,
                                     162531ULL, 2501726ULL};
        return tbl[n - 1];
    }

    /* The coefficients fit in signed 64‑bit, but the intermediate
     * values may exceed uint64_t.  Use a 128‑bit integer for the
     * Horner evaluation; this is safe on GCC/Clang and works on
     * C++17 builds as well.  When seq_t is 128‑bit or larger,
     * promotion is harmless. */
    __int128 res = ( (__int128)1048576 );
    res = res * n - 30277632;
    res = res * n + 406210560;
    res = res * n - 3319585920;
    res = res * n + 18136811049;
    res = res * n - 68048382318;
    res = res * n + 171628664735;
    res = res * n - 266425935930;
    res = res * n + 194935658400;

    /* final division */
    return (seq_t)(res / 2520);
}
