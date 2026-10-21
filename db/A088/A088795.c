/*
 * A088795  a(n) = -1/16-3*n^2/8+17*n/12+n^3/12+(-1)^n/16.
 * https://oeis.org/A088795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-3 - 18*n^2 + 68*n + 4*n^3 + 3*(-1)^n) / 48
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses direct arithmetic; works for all n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset starts at 1; value undefined for n < 1 */
    long long nn = (long long)n;    /* work in signed 64-bit for safety */
    long long sign = ((n & 1ULL) ? -1LL : 1LL);   /* (-1)^n */
    long long numerator = 4LL * nn * nn * nn
                       - 18LL * nn * nn
                       + 68LL * nn
                       - 3LL
                       + 3LL * sign;
    return (seq_t)(numerator / 48);
}
