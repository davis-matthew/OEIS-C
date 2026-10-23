/*
 * A049995  Number of arithmetic progressions of 4 or more positive integers,
 *          nondecreasing with sum <= n.
 * https://oeis.org/A049995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=4..n} Σ_{a=1..⌊n/m⌋} (⌊(n - m·a)/(m·(m-1)/2)⌋ + 1)
 * Range:     u8 n <= 59, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    if (n < 4u)
        return 0;
    int64_t n64 = n;
    for (idx_t m = 4; m <= n; ++m) {
        int64_t m64 = m;
        int64_t C = m64 * (m64 - 1) / 2;   // m·(m-1)/2
        idx_t Amax = n / m;                // ⌊n/m⌋
        for (idx_t a = 1; a <= Amax; ++a) {
            int64_t val = n64 - m64 * (int64_t)a;   // n - m·a, non‑negative
            int64_t dmax = val / C;                // ⌊(n - m·a)/C⌋
            total += (seq_t)(dmax + 1);            // number of d for this a
        }
    }
    return total;
}
