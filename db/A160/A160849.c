/*
 * A160849  Number of lines through at least 2 points of a 9 X n grid of
 *          points.
 * https://oeis.org/A160849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (f(9,n,1) – f(9,n,2)) / 2 * where f(m,n,k) = Σ (n – |k
 *            x|) · (m – |k y|) over all integers * -n < kx < n, -m < ky < m,
 *            gcd(|x|,|y|)=1, and (x,y)≠(0,0). * Here m = 9. The function is
 *            implemented by iterating only over * non‑negative x and y and
 *            accounting for their sign multiplicities
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Small greatest‑common‑divisor routine for non‑negative numbers. */
static unsigned long long gcd_uint(unsigned long long a, unsigned long long b)
{
    while (b != 0ULL) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sum f(9,n,k) for a fixed k = 1 or 2. */
static seq_t sum_f(idx_t n, idx_t k)
{
    if (n == 0) return 0;

    const idx_t M = 9;                      /* the fixed number of rows */
    idx_t max_x = (n - 1) / k;              /* floor((n‑1)/k)   */
    idx_t max_y = (M - 1) / k;              /* floor((8)/k)     */

    seq_t total = 0;

    for (idx_t x = 0; x <= max_x; ++x) {
        for (idx_t y = 0; y <= max_y; ++y) {
            if (x == 0 && y == 0) continue;                 /* skip origin */
            if (gcd_uint(x, y) != 1ULL) continue;            /* coprime check */

            idx_t nx = n - k * x;                           /* n – |k x| */
            idx_t my = M - k * y;                           /* m – |k y| */

            seq_t term = (seq_t)nx * (seq_t)my;
            seq_t mult = (x == 0 || y == 0) ? 2 : 4;         /* sign multiplicity */

            total += mult * term;
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 by the OEIS offset. */
    if (n == 0) return 0;

    seq_t f1 = sum_f(n, 1);
    seq_t f2 = sum_f(n, 2);
    return (f1 - f2) / 2;
}
