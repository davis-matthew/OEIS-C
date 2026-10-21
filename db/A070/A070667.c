/*
 * A070667  Smallest m in range 2..n-1 such that m^2 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m in [2, n-1] with m^2 ≡ 1 (mod n), or 1 if no
 *            such m
 * Range:     u8 n <= 16, u16 n <= 1741, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)          /* no candidate m: return 1 */
        return 1;

    for (idx_t m = 2; m < n; ++m) {
        seq_t sq = (seq_t)m * m;          /* use seq_t to avoid overflow */
        if (sq % (seq_t)n == 1)
            return (seq_t)m;              /* first such m is the answer */
    }
    return 1;                                 /* no m found */
}
