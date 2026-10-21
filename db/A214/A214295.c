/*
 * A214295  a(n) = 1 if n is a square, -1 if n is three times a square, 0
 *          otherwise.
 * https://oeis.org/A214295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 1 if n is a perfect square, -1 if n is 3 times a perfect
 *            square, 0 otherwise This function uses only integer arithmetic
 *            and a binary search to compute integer square roots
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 0xFFFFFFFFULL;   /* maximum sqrt of UINT64_MAX */
    idx_t result = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;

        if (sq == n) {
            return mid;
        } else if (sq < n) {
            result = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Check if n is a perfect square. */
    idx_t r = isqrt_u64(n);
    if (r * r == n) {
        return (seq_t)1;
    }

    /* Check if n is divisible by 3 and n/3 is a perfect square. */
    if (n % 3 == 0) {
        idx_t m = n / 3;
        idx_t s = isqrt_u64(m);
        if (s * s == m) {
            return (seq_t)(-1);
        }
    }
    return (seq_t)0;
}
