/*
 * A116589  a(n) = Sum{sqrt(n) < i <= n} i - Sum{1 <= i < sqrt(n)} i.
 * https://oeis.org/A116589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum{ sqrt(n) < i <= n } i – Sum{ 1 <= i < sqrt(n) } i
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 4, ... the sequence starts at offset 1 */

#include <stdbool.h>

/* triangular number: t(k) = k*(k+1)/2, computed in seq_t */
static inline seq_t tri(idx_t k)
{
    seq_t x = (seq_t)k;
    return x * (x + (seq_t)1) / (seq_t)2;
}

/* integer square root using Newton's method (floor(sqrt(n))) */
static inline idx_t isqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Sequence A116589 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, guard for 0 */

    idx_t m = isqrt(n);               /* floor(sqrt(n)) */

    /* part1 = sum_{i=m+1}^{n} i = tri(n) - tri(m) */
    seq_t part1 = tri(n) - tri(m);

    /* part2 = sum_{1 <= i < sqrt(n)} i
       If n is a perfect square we exclude i = m; otherwise we include it. */
    seq_t part2;
    if ((m * m) == n)
        part2 = tri(m - 1);         /* m >= 1 for n >= 1 */
    else
        part2 = tri(m);

    return part1 - part2;
}
