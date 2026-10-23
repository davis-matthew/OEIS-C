/*
 * A183900  Number of nondecreasing arrangements of n+3 numbers in 0..5 with
 *          each number being the sum mod 6 of three others.
 * https://oeis.org/A183900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 30*n^4 + 355*n^3 + 1710*n^2 - 2096*n - 6000) / 120
 *            for n > 4 a(1) = 3, a(2) = 24, a(3) = 130, a(4) = 364
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)130;
    if (n == 4) return (seq_t)364;

    /* Use 128‑bit arithmetic to avoid overflow for n up to ~10,000
       (n^5 can reach 10^20 which does not fit in 64 bits). */
    __int128 x = (__int128)n;

    __int128 num = x * x * x * x * x      /* n^5 */
                + (__int128)30 * x * x * x * x      /* 30*n^4 */
                + (__int128)355 * x * x * x         /* 355*n^3 */
                + (__int128)1710 * x * x            /* 1710*n^2 */
                - (__int128)2096 * x                /* -2096*n */
                - (__int128)6000;                  /* -6000 */

    return (seq_t)(num / (__int128)120);
}
