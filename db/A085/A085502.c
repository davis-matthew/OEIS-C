/*
 * A085502  Number of (unordered) ways of making change for n dollars using
 *          coins of denominations 1, 5, 10, 25, 50 and 100.
 * https://oeis.org/A085502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 1) (80 n^4 + 310 n^3 + 362 n^2 + 121 n + 6) / 6
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 49, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using 128‑bit arithmetic to avoid overflow on 64‑bit builds */
    unsigned __int128 u  = (unsigned __int128)n;
    unsigned __int128 u2 = u * u;
    unsigned __int128 u3 = u2 * u;
    unsigned __int128 u4 = u2 * u2;
    unsigned __int128 poly = 80 * u4 + 310 * u3 + 362 * u2 + 121 * u + 6;
    unsigned __int128 res  = (u + 1) * poly / 6;
    return (seq_t)res;
}
