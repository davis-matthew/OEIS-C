/*
 * A024202  a(n) = [ (3rd elementary symmetric function of S(n))/(first
 *          elementary symmetric function of S(n)) ], where S(n) = {first n+2
 *          odd positive integers}.
 * https://oeis.org/A024202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n+1)*(n^2+3*n+1)/6)
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic if available to avoid overflow */
#if defined(__SIZEOF_INT128__)
    __uint128_t nn   = n;
    __uint128_t prod = nn * (nn + 1);                     /* n*(n+1)   */
    prod *= (nn * nn + 3 * nn + 1);                       /* * (n^2+3n+1) */
    prod /= 6;  /* division is exact or truncates toward zero – same as floor for positives */
    return (seq_t)prod;
#else
    /* fallback – divides by 2 and 3 before multiplying to keep the product in range */
    seq_t a = n;
    seq_t b = n + 1;
    seq_t c = n * n + 3 * n + 1;

    /* divide one factor by 2 */
    if ((a & 1) == 0)          a >>= 1;
    else if ((b & 1) == 0)     b >>= 1;
    else                       c >>= 1;

    /* divide one factor by 3 */
    if (a % 3 == 0)            a /= 3;
    else if (b % 3 == 0)       b /= 3;
    else                       c /= 3;

    return a * b * c;
#endif
}
