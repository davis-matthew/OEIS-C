/*
 * A166524  a(n) = 9*n - a(n-1), with n>1, a(1)=1.
 * https://oeis.org/A166524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n + 9 + 23*(-1)^n)/4 (a(1)=1)
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against the undefined call a(0); return 0 as a safe default. */
    if (n == 0) return 0;

    /* Compute in 64‑bit unsigned arithmetic to avoid overflow for
       the ranges normally used in the tests (n ≤ 10 000).   */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long num;

    if (n & 1ULL)       /* n odd: (-1)^n = -1 */
        num = 18ULL * nn + 9ULL - 23ULL;
    else                /* n even: (-1)^n = +1 */
        num = 18ULL * nn + 9ULL + 23ULL;

    /* The numerator is always divisible by 4; integer division is exact. */
    return (seq_t)(num / 4ULL);
}
