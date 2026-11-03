/*
 * A116108  Squares that are equal to the sum of 3 consecutive cubes.
 * https://oeis.org/A116108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   compute squares that equal the sum of 3 consecutive cubes * by
 *            exhaustive search over integer m. * a(n) = s^2 where s^2 = m^3 +
 *            (m+1)^3 + (m+2)^3. * Return the n-th such square (1‑based
 *            index). If n exceeds the number * of solutions, return 0. The
 *            sequence is known to have only 4 terms
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 4, big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-02)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* static cache for the 4 solutions */
    static bool initialized = false;
    static size_t count = 0;
    static seq_t values[16];

    if (!initialized) {
        count = 0;
        /* Search over a reasonable range of m.
         * All solutions lie in -1 ≤ m ≤ 23; we search +/-1000 to be safe. */
        for (int64_t m = -1000; m <= 1000; ++m) {
            if (m < -1)            /* sum would be negative */
                continue;

            /* compute m^3 + (m+1)^3 + (m+2)^3 */
            int64_t m1 = m + 1;
            int64_t m2 = m + 2;
            seq_t sum = (seq_t)(m * m * m + m1 * m1 * m1 + m2 * m2 * m2);

            seq_t root = floor_sqrt(sum);
            if (root * root == sum) {
                values[count++] = sum;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > count)
        return (seq_t)0;
    return values[n - 1];
}
