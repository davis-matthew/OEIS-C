/*
 * A234263  Number of (n+1) X (5+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(n) = 3*a(n-1) + 6*a(n-2) - 24*a(n-3) + 4*a(n-4)
 *          + 36*a(n-5) - 24*a(n-6)   for n ≥ 8
 * Initial values:
 *   a(1)=560, a(2)=754, a(3)=1100, a(4)=1816,
 *   a(5)=3188, a(6)=6076, a(7)=11876
 */

#include <stdint.h>   /* for uint64_t, if needed */

static const seq_t init_vals[8] = {
    0,      /* unused, 1‑based indexing */
    560,    /* a(1) */
    754,    /* a(2) */
    1100,   /* a(3) */
    1816,   /* a(4) */
    3188,   /* a(5) */
    6076,   /* a(6) */
    11876   /* a(7) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1; harmless */

    if (n < 8)
        return init_vals[n];   /* direct lookup for n = 1…7 */

    /* Maintain the last six values a(n-6)…a(n-1) */
    seq_t vals[6];
    vals[0] = init_vals[2];  /* a(2) */
    vals[1] = init_vals[3];  /* a(3) */
    vals[2] = init_vals[4];  /* a(4) */
    vals[3] = init_vals[5];  /* a(5) */
    vals[4] = init_vals[6];  /* a(6) */
    vals[5] = init_vals[7];  /* a(7) */

    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t new_val =
              3 * vals[5]
            + 6 * vals[4]
            - 24 * vals[3]
            + 4 * vals[2]
            + 36 * vals[1]
            - 24 * vals[0];

        /* Shift the window */
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = vals[4];
        vals[4] = vals[5];
        vals[5] = new_val;
    }

    return vals[5];   /* a(n) */
}
