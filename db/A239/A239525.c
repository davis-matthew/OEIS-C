/*
 * A239525  For 0 <= n <= 100, a(n) is smallest integer N such that some
 *          positive x/N rounds to n%, with x > 0.
 * https://oeis.org/A239525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) = smallest N ≥ 1 such that * ceil(N*(2n-1)/200) ≤
 *            floor(N*(2n+1)/200) and the integer lies >0. * For n = 0 the
 *            minimal N is 200. This matches the definition * “a(n) = minimal
 *            N with some positive x/N rounding to n%”
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 : only multiples of 1/N ≤ 0.5/100 = 0.005
     * require N ≥ 200 to allow x = 1. */
    if (n == 0) {
        return (seq_t)200;
    }

    /* For n ≥ 1 there is always a solution with N ≤ 100.
     * We search up to 200 to be safe for all n. */
    for (idx_t N = 1; N <= 200; ++N) {
        /* Use 64‑bit arithmetic – (2n±1) ≤ 20001 for n ≤ 10 000,
         * so no overflow occurs.  For larger n the algorithm still
         * holds because N ≤ 200, but we rely on the problem limits. */
        int64_t num1 = (int64_t)N * ((int64_t)2 * (int64_t)n - 1); /* N*(2n-1) */
        int64_t num2 = (int64_t)N * ((int64_t)2 * (int64_t)n + 1); /* N*(2n+1) */

        /* ceil(num1/200) – works also for negative num1 (n = 0 already handled). */
        int64_t lower = (num1 >= 0) ? (num1 + 199) / 200 : num1 / 200;
        if (lower < 1)                /* enforce x > 0 */
            lower = 1;

        int64_t upper = num2 / 200;   /* floor(num2/200) */

        if (lower <= upper)
            return (seq_t)N;
    }

    /* Theoretically unreachable: for all n there is a solution with N ≤ 200. */
    return (seq_t)201;
}
