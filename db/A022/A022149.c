/*
 * A022149  Coordination sequence for root lattice B_7.
 * https://oeis.org/A022149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, for n > 0, a(n) = (968*n^6 - 1368*n^5 + 5420*n^4 -
 *            4080*n^3 + 4232*n^2 - 852*n + 90)/45
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 24, u64 n <= 974, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Use unsigned __int128 for intermediate calculations to avoid overflow */
    unsigned __int128 x  = (unsigned __int128)n;
    unsigned __int128 x2 = x * x;
    unsigned __int128 x3 = x2 * x;
    unsigned __int128 x4 = x3 * x;
    unsigned __int128 x5 = x4 * x;
    unsigned __int128 x6 = x5 * x;

    unsigned __int128 num =
        (unsigned __int128)968  * x6
        - (unsigned __int128)1368 * x5
        + (unsigned __int128)5420 * x4
        - (unsigned __int128)4080 * x3
        + (unsigned __int128)4232 * x2
        - (unsigned __int128)852  * x
        + (unsigned __int128)90;

    unsigned __int128 res = num / (unsigned __int128)45;
    return (seq_t)res;
}
