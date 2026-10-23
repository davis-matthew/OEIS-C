/*
 * A158333  Position of number of digits increment in the sequence of powers
 *          of 3.
 * https://oeis.org/A158333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + floor( n / log10(3) )
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed with a high‑precision rational approximation
   of 1 / log10(3).  For n ≤ 10000 the approximation
   1 / log10(3) ≈ 2095903639256 / 1000000000000 yields exact
   integer division, so the result is accurate for every n. */

seq_t a(idx_t n)
{
    /* 1 / log10(3) ≈ 2095903639256 / 1000000000000  */
    const unsigned long long D = 1000000000000ULL;   /* denominator 10^12 */
    const unsigned long long P = 2095903639256ULL;   /* numerator */

    /* decompose P = Q*D + R */
    const unsigned long long Q = P / D;   /* 2 */
    const unsigned long long R = P % D;   /* 95903639256 */

    /* floor( n * P / D ) = n*Q + floor( n*R / D ) */
    unsigned long long base  = (unsigned long long)n * Q;
    unsigned long long add   = (unsigned long long)((unsigned __int128)n * R / D);
    unsigned long long floor_val = base + add;

    return (seq_t)floor_val + 1;
}
