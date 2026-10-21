/*
 * A216671  Let S_k = {x^2+k*y^2: x,y nonnegative integers}. How many out of
 *          S_1, S_2, S_3, S_7 does n belong to?
 * https://oeis.org/A216671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k∈{1,2,3,7} such that n can be expressed as x²
 *            + k·y² with non‑negative integers x,y
 * Range:     u8 n <= 63, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                      /* for bool */
#include "../utils/floor_sqrt.h"           /* floor_sqrt(seq_t) */

/* Return true iff there exist integers x,y≥0 with n = x² + k·y² */
static bool
representable(idx_t n, unsigned k)
{
    /* y ranges from 0 up to floor(sqrt(n/k)) */
    seq_t  nk = (seq_t)n / (seq_t)k;
    idx_t  y_max = (idx_t)floor_sqrt(nk);

    seq_t  n_seq = (seq_t)n;
    seq_t  k_seq = (seq_t)k;

    for (idx_t y = 0; y <= y_max; ++y)
    {
        seq_t y_sq  = (seq_t)y * (seq_t)y;            /* y² */
        seq_t m     = n_seq - k_seq * y_sq;           /* n - k·y²  (≥0) */
        seq_t r     = floor_sqrt(m);                  /* floor(sqrt(m)) */
        if (r * r == m)                               /* perfect square */
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    unsigned count = 0;
    if (representable(n, 1)) ++count;
    if (representable(n, 2)) ++count;
    if (representable(n, 3)) ++count;
    if (representable(n, 7)) ++count;
    return (seq_t)count;
}
