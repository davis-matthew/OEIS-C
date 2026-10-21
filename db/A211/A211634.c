/*
 * A211634  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2<=x^2+y^2.
 * https://oeis.org/A211634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with 1≤w,x,y≤n and
 *            w^2≤x^2+y^2
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t acc = 0;
    for (idx_t x = 1; x <= n; ++x) {
        uint32_t x2 = (uint32_t)(x * x);
        uint32_t m   = 0;                     /* floor sqrt of x^2 + y^2, updated incrementally */
        for (idx_t y = 1; y <= x; ++y) {
            uint32_t sum = x2 + (uint32_t)(y * y);
            while ((uint32_t)(m + 1) * (uint32_t)(m + 1) <= sum) ++m;

            seq_t val = (m <= n) ? (seq_t)m : (seq_t)n;
            if (x == y) {
                acc += val;
            } else {
                acc += (seq_t)((unsigned long long)val * 2);
            }
        }
    }
    return acc;
}
