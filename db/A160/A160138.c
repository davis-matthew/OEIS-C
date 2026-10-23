/*
 * A160138  a(n) = number of solutions to the system: x + y + z + w = n, -2x -
 *          y + z + 2w = 5 with nonnegative x, y, z, w.
 * https://oeis.org/A160138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of non‑negative integer solutions of * x + y
 *            + z + w = n * -2x - y + z + 2w = 5 (the “x + y + z + w = n, -2x
 *            - y + z + 2w = 5” system) * * Derivation reduces the system to
 *            the inequalities * 3x + 2y ≥ n-5 * 4x + 3y ≤ 2n-5 * with x,y ≥
 *            0, and the count equals the number of integ...
 * Range:     u8 n <= 53, u16 n <= 884, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int64_t floor_div(int64_t a, int64_t b)
{
    if (a >= 0)
        return a / b;
    /* Floor division for negative a */
    return - ((-a + b - 1) / b);
}

seq_t a(idx_t n)
{
    /* Early exit for n that cannot satisfy the lower bound (n ≤ 2) */
    if (n <= 2)
        return 0;

    seq_t total = 0;

    /* Compute x upper bound: 4x ≤ 2n-5  →  x ≤ floor((2n-5)/4) */
    int64_t two_n_minus5 = (int64_t)2 * (int64_t)n - 5;
    idx_t x_max = (idx_t)floor_div(two_n_minus5, 4);
    /* If x_max is negative due to very small n, loop is skipped */
    if (x_max == (idx_t)UINT64_MAX)  /* floor_div returned negative */
        return 0;

    for (idx_t x = 0; x <= x_max; ++x)
    {
        /* Compute lower bound for y: 3x + 2y ≥ n-5  →  y ≥ ceil((n-5-3x)/2) */
        int64_t t = (int64_t)n - 5 - 3 * (int64_t)x;

        int64_t lower_y;
        if (t >= 0)
            lower_y = (t + 1) / 2;  /* ceil for non‑negative t */
        else
            lower_y = t / 2;        /* C truncates toward zero, which is ceil for negative t */

        if (lower_y < 0)
            lower_y = 0;            /* y must be non‑negative */

        /* Compute upper bound for y: 4x + 3y ≤ 2n-5  →  y ≤ floor((2n-5-4x)/3) */
        int64_t s = 2 * (int64_t)n - 5 - 4 * (int64_t)x;
        int64_t upper_y = floor_div(s, 3);

        if (upper_y < lower_y)
            continue;  /* No valid y for this x */

        total += (seq_t)(upper_y - lower_y + 1);
    }

    return total;
}
