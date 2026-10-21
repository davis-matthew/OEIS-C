/*
 * A064027  a(n) = (-1)^n*Sum_{d|n} (-1)^d*d^2.
 * https://oeis.org/A064027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = (-1)^n * Sum_{d|n} (-1)^d * d^2 (OEIS A064027)
 * Range:     u8 n <= 14, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function evaluates the divisor sum with an O(√n) loop.  All
 * intermediate arithmetic is done in signed 64‑bit variables; the final
 * value is cast to seq_t.  The result is always non‑negative for n≥1. */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* signed 64‑bit accumulator for the divisor sum */
    long long sum = 0;
    idx_t d;

    /* Enumerate divisor pairs (d , n/d) */
    for (d = 1; d <= n / d; ++d) {
        if ((n % d) != 0)
            continue;

        /* term for divisor d:  (-1)^d * d^2
           (-1)^d is -1 when d is odd, +1 when d is even. */
        long long val = (long long)d * (long long)d;
        if ((d & 1u) != 0)     /* d odd -> sign -1 */
            val = -val;
        sum += val;

        /* the paired divisor e = n/d, unless it equals d */
        idx_t e = n / d;
        if (e != d) {
            long long val2 = (long long)e * (long long)e;
            if ((e & 1u) != 0)    /* e odd -> sign -1 */
                val2 = -val2;
            sum += val2;
        }
    }

    /* Multiply by (-1)^n: if n is odd we negate the sum. */
    if ((n & 1u) != 0)
        sum = -sum;

    return (seq_t)sum;
}
