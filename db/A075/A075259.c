/*
 * A075259  Number of solutions (x,y,z) to 3/(2n+1) = 1/x + 1/y + 1/z
 *          satisfying 0 < x < y < z and odd x, y, z.
 * https://oeis.org/A075259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = number of solutions (x,y,z) to 3/(2n+1) = 1/x + 1/y + 1/z
 *            with 0 < x < y < z and odd x, y, z
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 275, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* external integer types
 *  - idx_t   (unsigned 64‑bit)  for indices
 *  - seq_t   (unsigned, arbitrary precision) for sequence values
 */

/* Helper to get next odd number >= v */
static inline idx_t next_odd(idx_t v)
{
    if ((v & 1) == 0) ++v;
    return v;
}

/* a(idx_t n): count solutions for given n >= 1 */
seq_t a(idx_t n)
{
    /* Using seq_t for all arithmetic to avoid overflow */
    seq_t denom = (seq_t)n * 2 + 1;          /* 2n+1 */
    seq_t count = 0;

    /* minimal x > denom/3 */
    idx_t x_start = (idx_t)(denom / 3) + 1;
    x_start = next_odd(x_start);

    for (idx_t x = x_start; x < (denom + 1); x += 2)        /* x odd */
    {
        seq_t sx = (seq_t)x;
        seq_t a_val = 3 * sx - denom;        /* a = 3x - (2n+1) > 0 */

        /* lower bound for y : y > denom*x / a_val */
        seq_t y_min_real = (denom * sx) / a_val;   /* floor division */
        idx_t y_min = (idx_t)(y_min_real) + 1;      /* strict > */
        idx_t y_start = x + 2;                     /* y > x */

        if (y_min > y_start) y_start = y_min;
        y_start = next_odd(y_start);

        /* upper bound for y : y < (2*denom*x)/a_val */
        seq_t double_den = (seq_t)2 * denom * sx;
        idx_t y_upper = (idx_t)((double_den - 1) / a_val); /* floor((2*denom*x)/a_val - ε) */

        if (y_start > y_upper) continue;

        for (idx_t y = y_start; y <= y_upper; y += 2)
        {
            seq_t sy = (seq_t)y;

            /* D = 3xy - denom(x+y) = a_val * y - denom * x */
            seq_t D = a_val * sy - denom * sx;
            if (D == 0) continue; /* avoid division by zero */

            seq_t num = denom * sx * sy;    /* numerator for z */
            if (num % D != 0) continue;     /* z not integer */

            seq_t z = num / D;
            if (z <= sy) break;             /* further y will only reduce z */

            if ((z & 1) == 0) continue;     /* z must be odd */

            ++count;
        }
    }

    return count;
}
