/*
 * A186383  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=5i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186384.
 * https://oeis.org/A186383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( ( sqrt(1+40*n) - 1 ) / 2 ) * - (is_triangular
 *            ? 1 : 0) * where is_triangular = 1+40*n is a perfect square *
 *            (this subtracts one when 5*n is a triangular number)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned integers.
 * Returns floor(sqrt(x)). */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL;   /* sqrt(ULLONG_MAX) < 2^32 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq  = mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour for n<1 is undefined */
    uint64_t n64 = (uint64_t)n;

    /* D = 1 + 40*n  (always fits in 64 bits for the intended range) */
    uint64_t D = 1ULL + 40ULL * n64;

    uint64_t s = isqrt_uint64(D);
    uint64_t t = (s - 1) / 2;         /* floor( (sqrt(D)-1)/2 ) */

    seq_t result = (seq_t)n + (seq_t)t;

    if (s * s == D) /* 5*n is triangular, subtract 1 */
        result -= 1;

    return result;
}
