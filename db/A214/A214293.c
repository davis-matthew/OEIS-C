/*
 * A214293  a(n) = 1 if n is a square, -1 if n is five times a square.
 * https://oeis.org/A214293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = 1 if n is a perfect square, -1 if n is 5 times a perfect
 *            square, 0 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for unsigned 64‑bit values.
   Uses only integer arithmetic and guaranteed to fit in idx_t. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL;   /* maximum possible square root of a 64‑bit
                                    unsigned integer */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* upper middle to finish */
        if ((n / mid) >= mid)   /* mid*mid <= n  <=>  mid <= n/mid  */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, but guard */

    idx_t s = isqrt_u64(n);
    if (s * s == n) return (seq_t)1;      /* perfect square */

    if (n % 5 == 0) {
        idx_t t = n / 5;
        idx_t r = isqrt_u64(t);
        if (r * r == t) return (seq_t)-1; /* 5 times a perfect square */
    }

    return (seq_t)0;                      /* all other cases */
}
