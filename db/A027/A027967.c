/*
 * A027967  T(n, 2*n-5), T given by A027960.
 * https://oeis.org/A027967
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (840 - 736*n + 300*n^2 - 45*n^3 + n^5) / 120
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 220, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A027967 is defined for n ≥ 3; results for smaller n are unspecified. */
    if (n < 3)
        return (seq_t)0;

    /* Use signed __int128 to safely compute the polynomial for n up to ~10^4
       (or larger) while keeping intermediate results within bounds. */
    __int128 tn = (__int128)n;
    __int128 res = 840;
    res -= 736 * tn;
    res += 300 * tn * tn;
    res -= 45  * tn * tn * tn;
    res += tn * tn * tn * tn * tn;
    res /= 120;

    return (seq_t)res;
}
