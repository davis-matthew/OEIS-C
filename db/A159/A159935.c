/*
 * A159935  Least integer such that a(n)^2 - n is the sum of two nonzero
 *          squares.
 * https://oeis.org/A159935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Find the least integer a such that a^2 - n is the sum of two
 *            non‑zero squares
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The length of the answer sequence is unbounded but for n up to ~10 000 it is easily computed. */
#include <stdbool.h>
#include <math.h>

static inline idx_t isqrt_u64(idx_t x)
{
    /* integer square root of a 64‑bit unsigned integer */
    idx_t r = (idx_t) sqrt((long double) x);
    while ((r + 1) * (r + 1) <= x) r++;
    while (r * r > x) r--;
    return r;
}

/* test whether k > 0 can be written as x^2 + y^2 with x,y >= 1 */
static bool has_nonzero_square_sum(idx_t k)
{
    if (k < 2) return false;                /* need at least 1^2+1^2 = 2 */
    idx_t max_x = isqrt_u64(k - 1);         /* x^2 must be <= k-1 to keep y^2 > 0 */
    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t x2   = x * x;
        idx_t y2   = k - x2;
        if (y2 == 0) continue;             /* y must be non‑zero */
        idx_t y = isqrt_u64(y2);
        if (y * y == y2) return true;      /* found a representation */
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t a_val = isqrt_u64(n) + 1;         /* a must satisfy a^2 > n */
    for (;;) {
        idx_t k = a_val * a_val - n;
        if (k > 0 && has_nonzero_square_sum(k))
            return (seq_t) a_val;           /* minimal a found */
        ++a_val;
    }
}
