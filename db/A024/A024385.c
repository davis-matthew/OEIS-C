/*
 * A024385  a(n) = [ (2nd elementary symmetric function of S(n))/(first
 *          elementary symmetric function of S(n)) ], where S(n) = {first n+1
 *          positive integers congruent to 1 mod 4}.
 * https://oeis.org/A024385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (3*(n+1)*(2n+1)^2 - 3*(4*(n+1)-3) - 8*n*(2n+1)) /
 *            (6*(2n+1)) )
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Temporaries are 64‑bit to avoid overflow for all test ranges. */
    unsigned long long A = (unsigned long long)n + 1UL;                 /* n+1 */
    unsigned long long B = 2ULL * (unsigned long long)n + 1UL;           /* 2n+1 */

    unsigned long long term1 = 3ULL * A * B * B;                   /* 3*A*B^2 */
    unsigned long long term2 = 3ULL * (4ULL * A - 3ULL);             /* 3*(4A-3) */
    unsigned long long term3 = 8ULL * (unsigned long long)n * B;      /* 8*n*B */

    unsigned long long num = term1 - term2 - term3;                 /* numerator */
    unsigned long long den = 6ULL * B;                              /* denominator */

    return (seq_t)(num / den); /* integer division truncates toward zero */
}
