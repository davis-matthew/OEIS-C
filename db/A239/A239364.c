/*
 * A239364  Numbers n such that (n^2-4)/10 is a square.
 * https://oeis.org/A239364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*a(n-1) - a(n-2) with a(1)=38, a(2)=1442 The function
 *            computes the terms by iterating the recurrence, using a 128‑bit
 *            intermediate where available to avoid overflow on 64‑bit builds.
 *            Works with arbitrary‑precision seq_t as well
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n = 0 is undefined in the OEIS offset; return 0 for safety */
    if (n == 0)
        return 0;

    /* Return the first two terms directly */
    if (n == 1)
        return (seq_t)38;
    if (n == 2)
        return (seq_t)1442;

    /* iteratively compute the recurrence */
    seq_t prev2 = (seq_t)38;   /* a(1) */
    seq_t prev1 = (seq_t)1442; /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
        /* Use 128‑bit intermediate on 64‑bit C builds */
        __uint128_t prod = (__uint128_t)prev1 * 38;
        cur = (seq_t)(prod - prev2);
#else
        /* For arbitrary‑precision seq_t or when 128‑bit is not available */
        cur = prev1 * 38 - prev2;
#endif
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
