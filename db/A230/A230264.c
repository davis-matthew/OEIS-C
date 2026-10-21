/*
 * A230264  Number of nonnegative integer solutions to the equation x^2 -
 *          9*y^2 = n.
 * https://oeis.org/A230264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions (x, y) to the
 *            equation * x^2 - 9·y^2 = n * This implementation enumerates all
 *            divisors f of n with f^2 ≤ n and tests * the condition n - f^2 ≡
 *            0 (mod 6f), which follows from the general solution * x = 3·y +
 *            f, y = (n - f^2)/(6f). For each such diviso...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t but idx_t and seq_t are provided by the build */

/* Integer square root for a 64‑bit unsigned value, using the Babylonian
 * algorithm.  This routine returns the largest integer r such that r^2 ≤ n.
 */
static idx_t isqrt_u64(idx_t n)
{
    if (n == 0 || n == 1)
        return n;
    idx_t r = n;
    while (1) {
        idx_t q = n / r;
        idx_t next = (r + q) >> 1;
        if (next >= r)
            return r;
        r = next;
    }
}

/* Number of solutions to x^2 - 9·y^2 = n with x, y ≥ 0. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)        /* the OEIS offset is 1, but a(0) is mathematically 0 */
        return 0;

    idx_t maxf = isqrt_u64(n);

    for (idx_t f = 1; f <= maxf; ++f) {
        if (n % f != 0)
            continue;                 /* f must divide n */

        /* Since f ≤ sqrt(n), the subtraction cannot underflow. */
        idx_t tmp   = n - f * f;
        idx_t denom = 6 * f;          /* 6·f ≤ 6·sqrt(n) < 2^64 for all n */

        if (tmp % denom == 0)
            ++count;                  /* valid solution found */
    }

    return count;
}
