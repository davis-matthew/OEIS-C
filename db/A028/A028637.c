/*
 * A028637  Expansion of (theta_3(z)*theta_3(18z)+theta_2(z)*theta_2(18z)).
 * https://oeis.org/A028637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y) to x^2 + 18*y^2 = n *
 *            with x ≡ y (mod 2). This equals the coefficient of q^n in *
 *            (theta_3(z)*theta_3(18z)+theta_2(z)*theta_2(18z))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* Maximum |y| such that 18*y^2 <= n. */
    idx_t y_max = _floor_isqrt_u64(n / 18);

    for (idx_t y = 0; y <= y_max; ++y) {
        idx_t y2  = y * y;          /* y^2 fits in idx_t for the given bound. */
        idx_t prod = 18 * y2;        /* 18*y^2 <= n, so prod fits in idx_t. */
        idx_t t = n - prod;         /* Remaining part for x^2. */
        idx_t s = _floor_isqrt_u64(t);

        if (s * s != t)              /* t is not a perfect square. */
            continue;

        if ((s & 1) != (y & 1))      /* parity mismatch: skip. */
            continue;

        if (y == 0) {
            /* y = 0: one or two x solutions, depending on whether s = 0. */
            result += (s == 0 ? (seq_t)1 : (seq_t)2);
        } else {
            /* y > 0: both signs of y are valid. */
            if (s == 0) {
                /* Only x = 0 gives a solution; each sign of y gives one. */
                result += (seq_t)2;
            } else {
                /* Two x (±s) for each of two signs of y. */
                result += (seq_t)4;
            }
        }
    }
    return result;
}
