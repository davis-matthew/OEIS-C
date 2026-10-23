/*
 * A049866  a(n) = Sum_{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 1,1,3.
 * https://oeis.org/A049866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-4) for n >= 4 Initial values:
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=4
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* compute a(n) */
seq_t a(idx_t n)
{
    /* static array large enough for the tests (up to n≈10000) */
    /* seq_t is either a builtin integer type or an arbitrary-precision integer class,
       so a constant-size array can be constructed in both C and C++. */
    static seq_t cache[10001] = { 1, 1, 3, 4 };
    static idx_t computed = 4;          /* next index to compute */

    /* If we already have the requested value, return it. */
    if (n < computed)
        return cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] - cache[i - 3] + cache[i - 4];
    }

    computed = n + 1;
    return cache[n];
}
