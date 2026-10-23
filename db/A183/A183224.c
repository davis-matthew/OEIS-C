/*
 * A183224  Complement of the 12-gonal numbers.
 * https://oeis.org/A183224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(1/2 + sqrt((n+1)/5))
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses integer arithmetic only and works for all provided types
   (idx_t is an unsigned 64‑bit integer, seq_t is any unsigned integer
   type or an arbitrary‑precision class). */

#include <stdint.h>

/* Integer floor square root for 64‑bit unsigned values.
   No floating point is used. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* highest power of two such that (bit << 1) <= x*4  (i.e. 4^k) */
    idx_t bit = 1ULL << 62;
    while (bit > x)
        bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* term a(n) for n >= 1 works for all n. */
    if (n == 0)   /* undefined by the OEIS, but return 0 for safety */
        return (seq_t)0;

    /* t = n + 1, the argument of the parenthesis. */
    idx_t t = n + 1;

    /* s = floor(sqrt(t/5)), computed by integer division. */
    idx_t s = isqrt_u64(t / 5);

    /* Determine whether the fractional part of sqrt(t/5) is ≥ 0.5.
       Increment s if so.  The condition is equivalent to
           t >= 5*s^2 + 5*s + 2
       which is guaranteed to stay within 64‑bit arithmetic for all
       possible n. */
    idx_t threshold = 5 * s * s + 5 * s + 2;
    if (t >= threshold)
        ++s;

    return (seq_t)n + (seq_t)s;
}
