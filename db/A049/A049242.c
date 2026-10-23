/*
 * A049242  Smallest nonnegative value taken on by 3x^2 - ny^2 for an infinite
 *          number of integer pairs (x, y).
 * https://oeis.org/A049242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest non‑negative integer k such that the equation
 *            3*x*x - n*y*y = k has infinitely many integer solutions (x,y).
 *            The implementation relies on the following observations: - 0 is
 *            achievable infinitely often iff n = 3*m*m. - For n % 3 == 1 the
 *            value 1 can never be represented; we only ha...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static bool has_solution(idx_t n, int k)
{
    /* Search y in a moderate range; this is safe for n ≤ 10000. */
    const idx_t MAX_Y = 20000;
    for (idx_t y = 0; y <= MAX_Y; ++y)
    {
        uint64_t y2 = (uint64_t)y * (uint64_t)y;
        uint64_t t = (uint64_t)k + (uint64_t)n * y2;
        if (t % 3 != 0) continue;
        uint64_t s = t / 3;
        /* integer square root via double and adjustment */
        uint64_t r = (uint64_t)sqrt((long double)s);
        while ((r + 1) * (r + 1) <= s) ++r;
        while (r * r > s) --r;
        if (r * r == s) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Case 0: infinite solutions with k = 0 */
    if (n % 3 == 0) {
        uint64_t m2 = n / 3;
        uint64_t r = (uint64_t)sqrt((long double)m2);
        while ((r + 1) * (r + 1) <= m2) ++r;
        while (r * r > m2) --r;
        if (r * r == m2) return (seq_t)0;
        return (seq_t)3;            /* no 1 or 2 possible when n % 3 == 0 */
    }

    /* n % 3 == 1  -->  test k = 2 */
    if (n % 3 == 1) {
        if (has_solution(n, 2)) return (seq_t)2;
        return (seq_t)3;
    }

    /* n % 3 == 2  -->  test k = 1 */
    if (has_solution(n, 1)) return (seq_t)1;
    return (seq_t)3;
}
