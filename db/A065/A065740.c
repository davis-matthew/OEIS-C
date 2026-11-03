/*
 * A065740  Largest square <= n^n.
 * https://oeis.org/A065740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(sqrt(n^n))^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 98 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
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
    if (n == 0) return (seq_t)0;   /* not used, offset is 1 */

    /* cast index to seq_t to avoid intermediate overflow on the index type */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)n;

    /* compute n^n */
    seq_t pow_n = ipow(base, exp);

    /* largest integer <= sqrt(n^n) */
    seq_t root = floor_sqrt(pow_n);

    /* return the square of that integer */
    return root * root;
}
