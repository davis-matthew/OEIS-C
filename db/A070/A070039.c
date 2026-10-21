/*
 * A070039  Sum of the divisors of n that are < sqrt(n).
 * https://oeis.org/A070039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<sqrt(n)} d
 * Range:     u8 n <= 1679, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
/* Compute floor(sqrt(n)) for n up to 64‑bit */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 is the first term; n==0 is unused */
        return 0;

    idx_t r = _floor_isqrt_u64(n);   /* floor sqrt(n) */

    seq_t sum = 0;
    /* add all divisors i <= r */
    for (idx_t i = 1; i <= r; ++i) {
        if (n % i == 0)
            sum += (seq_t)i;
    }

    /* exclude sqrt(n) itself if n is a perfect square */
    if (r * r == n)
        sum -= (seq_t)r;

    return sum;
}
