/*
 * A135051  Pyramid game person numbers that have integer solutions.
 * https://oeis.org/A135051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = 9 - 2*(-1)^n + 4*(-8+(-1)^n)*n + 32*n^2
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 468, u64 n <= 468,
 *            u128 n <= 468, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = (seq_t)n;            /* convert index to seq_t */
    seq_t sq = 32 * un * un;       /* 32*n^2  (always fits in seq_t for n ≤ 10000) */

    /* Decide which quadratic branch to use */
    if ((n & 1ULL) == 0) /* even n: 32n^2 - 28n + 7 */
    {
        return sq - 28 * un + 7;
    }
    else                /* odd n:  32n^2 - 36n + 11 */
    {
        return sq - 36 * un + 11;
    }
}
