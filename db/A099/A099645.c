/*
 * A099645  Number of iterations until n reaches a number in A039943 under the
 *          sum-of-squares-of-digits map A003132.
 * https://oeis.org/A099645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = #iterations of the sum–of–squares–of–digits map f(n)=Σ
 *            dᵢ² until n reaches a cycle number (1 or
 *            4,16,37,58,89,145,42,20)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 841 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* local helper: test whether x is a member of the 89‑cycle or the fixed
   point 1. */
static inline bool is_cycle_member(uint64_t x)
{
    return x == 1 || x == 4 || x == 16 || x == 37 ||
           x == 58 || x == 89 || x == 145 || x == 42 ||
           x == 20;
}

/* compute Σ dᵢ² for a non‑negative 64‑bit integer x. */
static inline uint64_t digit_square_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

/* external linkage: the OEIS sequence function. */
seq_t a(idx_t n)
{
    /* offset 1: ignore n == 0 but handle it safely. */
    if (n == 0)
        return 0;

    uint64_t cur = (uint64_t)n;
    seq_t steps = 0;

    while (!is_cycle_member(cur)) {
        cur = digit_square_sum_uint64(cur);
        ++steps;
    }
    return steps;
}
