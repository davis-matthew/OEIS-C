/*
 * A028609  Expansion of (theta_3(z)*theta_3(11z) + theta_2(z)*theta_2(11z)).
 * https://oeis.org/A028609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer solutions (x, y) to x^2 + x*y + 3*y^2 =
 *            n
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root for 64‑bit inputs (works for all arguments
   used in this function). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* n may be zero */
    if (n == (idx_t)0) return (seq_t)1;

    /* upper bound for |y|: 4*n - 11*y^2 >= 0  =>  y^2 <= 4*n/11 */
    idx_t limit = isqrt_u64((4 * n) / 11);

    seq_t count = 0;

    /* iterate y from -limit to +limit */
    for (int y = -(int)limit; y <= (int)limit; ++y)
    {
        idx_t ay = (idx_t)((y >= 0) ? y : -y);
        idx_t t = 4 * n - 11 * ay * ay;
        if (t < (idx_t)0) continue;            /* should not happen */
        idx_t k = isqrt_u64(t);
        if (k * k != t) continue;              /* not a perfect square */
        if (((k + y) & 1) != 0) continue;      /* x would be non‑integer */

        if (k == (idx_t)0)
            count += (seq_t)1;                  /* double root */
        else
            count += (seq_t)2;                  /* two distinct roots */
    }

    return count;
}
