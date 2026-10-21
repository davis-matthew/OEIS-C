/*
 * A061228  a(1) = 2, a(n) = smallest number greater than n that is not
 *          coprime to n.
 * https://oeis.org/A061228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer > n that shares a factor with n. For
 *            even n, a(n) = n + 2; For odd n, a(n) = n + smallest divisor > 1
 *            (or 2n if n is prime)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n <= 1)                        /* a(1) = 2 */
        return (seq_t)2;

    /* Even numbers: the smallest non-coprime integer is n + 2. */
    if ((n & 1) == 0)
        return (seq_t)n + (seq_t)2;

    /* Odd numbers: check divisibility by odd divisors up to sqrt(n). */
    idx_t limit = isqrt_u64(n);
    for (idx_t d = 3; d <= limit; d += 2)
    {
        if (n % d == 0)
            return (seq_t)n + (seq_t)d;
    }

    /* n is an odd prime (or >1 and not divisible by any smaller odd d). */
    return (seq_t)n + (seq_t)n;       /* a(n) = 2n */
}
