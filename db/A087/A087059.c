/*
 * A087059  Difference between 2*n^2 and the next greater square number.
 * https://oeis.org/A087059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + floor(n*sqrt(2)))^2 - 2*n^2
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for n ≥ 1.
   Uses only 64‑bit intermediate arithmetic and no external sqrt routine. */
seq_t a(idx_t n)
{
    /* offset 1 – the function is never called with n = 0 in the tests */
    if (n == 0)
        return (seq_t)0;

    /* Work in 64‑bit unsigned integers; this is safe for all n
       used in the test harness (up to about 10 000). */
    uint64_t nn   = (uint64_t)n;
    uint64_t val  = 2 * nn * nn;            /* 2*n^2  */

    /* Integer square‑root of val by binary search. */
    uint64_t lo = 0, hi = 1;
    while (hi * hi <= val)          /* grow upper bound */
        hi <<= 1;

    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= val)
            lo = mid;
        else
            hi = mid - 1;
    }
    uint64_t r = lo;                 /* floor(sqrt(val)) = floor(n*sqrt(2)) */

    uint64_t next = (r + 1) * (r + 1);
    uint64_t diff = next - val;      /* (1+r)^2 - 2*n^2 */

    return (seq_t)diff;
}
