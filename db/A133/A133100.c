/*
 * A133100  Expansion of f(x, x^4) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A133100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 or there exists k ≥ 1 such that n = (5k^2 –
 *            3k)/2 or n = (5k^2 + 3k)/2; otherwise a(n) = 0. This follows
 *            from f(x,x^4) = 1 + Σ_{k≥1} x^{5k(k‑1)/2}(x^k + x^{4k})
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root by Newton’s method, returns floor(sqrt(n)). */
static idx_t int_sqrt(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* 2n = 5k^2 ± 3k  ⇒  40n + 9 must be a perfect square (D = s²). */
    idx_t D = 9 + 40 * n;
    idx_t s = int_sqrt(D);
    if (s * s != D) return (seq_t)0;

    /* Check the two quadratic solutions for k. */
    if ((s >= 3) && ((s - 3) % 10 == 0)) return (seq_t)1;
    if ((s + 3) % 10 == 0)                   return (seq_t)1;

    return (seq_t)0;
}
