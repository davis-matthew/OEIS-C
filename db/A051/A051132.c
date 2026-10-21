/*
 * A051132  Number of ordered pairs of integers (x,y) with x^2+y^2 < n^2.
 * https://oeis.org/A051132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = |{(x,y) ∈ ℤ² : x² + y² < n²}|, i.e. the number of integer
 *            lattice points strictly inside the circle of radius n. a(0) = 0
 * Range:     u8 n <= 9, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: integer square root, floor(sqrt(n)) for 0 ≤ n < 2⁶⁴ */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = (uint64_t)1 << 62;          /* highest power of four ≤ n */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* required external linkage */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* n and x are idx_t (uint64_t), safe to square for n ≤ 3e9 */
    uint64_t n2 = n * n;
    seq_t total = 0;

    for (idx_t x = 0; x < n; ++x) {
        uint64_t limit = n2 - x * x;          /* > 0 for 0 ≤ x < n */
        uint64_t ymax = isqrt_u64(limit - 1); /* floor sqrt of (limit-1) */
        seq_t count_y = (seq_t)2 * ymax + 1;   /* count of y for this x */

        if (x == 0)
            total += count_y;                 /* x = 0 occurs once */
        else
            total += (seq_t)2 * count_y;      /* x and –x both occur */
    }

    return total;
}
