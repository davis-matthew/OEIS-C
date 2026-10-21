/*
 * A228172  Number of integer pairs (x,y) such that 0<=y<=x, x>0, and
 *          x^2+y^2<=n^2.
 * https://oeis.org/A228172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) with 0 <= y <= x, x>0, *
 *            and x² + y² <= n². This is equivalent to a(n) = A211340(n) + n
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values.
 * Returns floor(sqrt(v)). */
static idx_t int_sqrt_u64(idx_t v)
{
    idx_t bit = 1ULL << 62;                /* highest power of four <= 2⁶⁴ */
    while (bit > v) bit >>= 2;
    idx_t res = 0;
    while (bit) {
        if (v >= res + bit) {
            v -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Offset: a(1) is the first term. */
    if (n == 0) return 0u;

    idx_t nsq = (idx_t)n * (idx_t)n;   /* n² in a 64‑bit type */
    seq_t sum = 0;

    for (idx_t x = 1; x <= n; ++x) {
        idx_t xsq = x * x;
        idx_t diff = nsq - xsq;           /* n² - x² >= 0 */
        idx_t r = int_sqrt_u64(diff);     /* floor sqrt(n² - x²) */
        idx_t y_max = r < x ? r : x;      /* y <= x and y ≤ sqrt(...) */
        seq_t add = (seq_t)(y_max + 1);   /* include y = 0 */
        sum += add;
    }
    return sum;
}
