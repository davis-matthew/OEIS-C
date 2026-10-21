/*
 * A218799  Number of solutions to x^2 + 2y^2 = n^2.
 * https://oeis.org/A218799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x, y) ∈ ℕ^2 : x^2 + 2 y^2 = n^2}| (counting each pair
 *            with x ≥ 0, y ≥ 0 only once)
 * Range:     u8 n <= 7, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type used for the sequence. */
    seq_t N   = (seq_t)n;
    seq_t Nsq = N * N;                     /* n^2   */

    /* The maximum y satisfies 2 y^2 ≤ n^2, i.e. y ≤ floor( sqrt(n^2/2) ). */
    seq_t half   = Nsq / 2;
    seq_t max_y  = floor_sqrt(half);      /* floor sqrt, may overflow if seq_t too small, but is safe for expected ranges */

    seq_t count = 0;

    for (idx_t yi = 0; yi <= (idx_t)max_y; ++yi)
    {
        seq_t y   = (seq_t)yi;
        seq_t y2  = y * y;
        seq_t two_y2 = y2 << 1;            /* 2 * y^2 */
        if (two_y2 > Nsq) break;          /* safety guard */

        seq_t rem = Nsq - two_y2;
        seq_t xr  = floor_sqrt(rem);
        if (xr * xr == rem)
            count += 1;                   /* one solution found */
    }

    return count;
}
