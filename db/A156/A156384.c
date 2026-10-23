/*
 * A156384  The number of solutions to x^2 + y^2 + 2*z^2 = n in nonnegative
 *          integers x,y,z.
 * https://oeis.org/A156384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computation: brute‑force enumeration over all z and x.
   For each n it counts the non‑negative integer solutions of
   x^2 + y^2 + 2*z^2 = n. */

static inline seq_t isqrt_seq(seq_t x)
{
    if (x == 0)
        return 0;
    seq_t lo = 1, hi = x;
    while (lo <= hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t div = x / mid;                /* avoid overflow in mid*mid  */
        if (mid > div)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t total = 0;

    for (idx_t z = 0; ; ++z)
    {
        seq_t z_sq  = (seq_t)z * (seq_t)z;
        seq_t two_z_sq = z_sq * 2;

        if (two_z_sq > n_seq)
            break;

        seq_t m = n_seq - two_z_sq;          /* remaining to be written as x² + y² */

        seq_t sqrt_m_seq = isqrt_seq(m);
        idx_t sqrt_m = (idx_t)sqrt_m_seq;

        seq_t xy_count = 0;
        for (idx_t x = 0; x <= sqrt_m; ++x)
        {
            seq_t x_sq = (seq_t)x * (seq_t)x;
            if (x_sq > m)
                break;

            seq_t y_sq = m - x_sq;
            seq_t sqrt_y_seq = isqrt_seq(y_sq);
            if (sqrt_y_seq * sqrt_y_seq == y_sq)
                ++xy_count;
        }

        total += xy_count;
    }

    return total;
}
