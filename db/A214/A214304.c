/*
 * A214304  Expansion of phi(q) + phi(q^2) - phi(q^4) in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A214304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n==0 else 2 * (is_square(n) + is_square(n/2) -
 *            is_square(n/4))
 * Range:     u8 n <= 9808, u16 n <= 9808, u32 n <= 9808, u64 n <= 9808,
 *            u128 n <= 9808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the Lambert series  Σ x^{k^2} + x^{2k^2} - x^{4k^2}. */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values (floor). */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0 || n == 1)
        return n;
    idx_t low  = 1;
    idx_t high = (idx_t)1 << 32;          /* √(2^64‑1) < 2^32   */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* φ(1) + φ(1) – φ(1)  = 1 */
        return 1;

    seq_t val = 0;

    /* square root of n itself */
    idx_t r = isqrt_u64(n);
    if (r * r == n)
        val += 2;

    /* square root of n/2  (only if n is even) */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;
        idx_t s = isqrt_u64(m);
        if (s * s == m)
            val += 2;
    }

    /* square root of n/4  (only if n is divisible by 4) */
    if ((n & 3) == 0) {
        idx_t m = n >> 2;
        idx_t s = isqrt_u64(m);
        if (s * s == m)
            val -= 2;
    }

    return val;
}
