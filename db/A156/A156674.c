/*
 * A156674  Numbers k such that k^2 - 2 == 0 (mod 49).
 * https://oeis.org/A156674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*n/2 - 49/4 + 9*(-1)^n/4 (OEIS A156674)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use signed 64‑bit intermediate to evaluate the rational formula.
       The result is always non‑negative for n >= 1 and fits into seq_t
       for all n used in the tests. */
    int64_t N = (int64_t)n;
    int64_t part1 = (49 * N) / 2;                       /* 49*n/2 */
    int64_t part2 = 49 / 4;                              /* 49/4 */
    int64_t sign  = (N % 2 == 0) ? 1 : -1;               /* (-1)^n */
    int64_t part3 = (9 * sign) / 4;                      /* 9*(-1)^n/4 */
    int64_t res   = part1 - part2 + part3;               /* assembled result */

    return (seq_t)res;
}
