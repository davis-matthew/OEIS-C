/*
 * A063038  a(n) = floor(n*sqrt(n)) - d(n), where d(n) is the number of
 *          divisors function.
 * https://oeis.org/A063038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n * sqrt(n)) - d(n)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(n) is the number of positive divisors of n. */
#include <stdint.h>
#include <stdbool.h>

#include "../utils/ipow_i32.h"
#include "../utils/num_divisors.h"

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

seq_t a(idx_t n) {
    /* Convert the index to seq_t to perform arithmetic in the correct type. */
    seq_t nn = (seq_t)n;
    /* Compute n^3.  For the intended range (up to about 10⁴) this fits in seq_t
       for the 64‑bit build and works for arbitrary‑precision builds. */
    seq_t n3 = nn * nn * nn;
    /* floor(n * sqrt(n)) = floor(sqrt(n^3)) */
    seq_t val = floor_sqrt(n3);
    /* Number of divisors of n.  Since n ≤ 2^64−1, this function returns a value
       that fits in seq_t on all supported builds. */
    seq_t divs = (seq_t)num_divisors(n);
    /* The desired sequence value. */
    return val - divs;
}
