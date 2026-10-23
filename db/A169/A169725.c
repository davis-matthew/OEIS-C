/*
 * A169725  a(n) = 3^(n-1)*(6*3^(n-1) + 5) + 1.
 * https://oeis.org/A169725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1) * (6 * 3^(n-1) + 5) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Based on the explicit expression given in the OEIS entry for A169725. */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n < 1, return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Let t = 3^(n-1).  The helper pow3 returns a seq_t. */
    seq_t t = pow3(n - 1);

    /* Compute a(n) = t * (6 * t + 5) + 1.  All intermediate operations
       are performed in seq_t, ensuring exactness for arbitrary‑precision
       builds. */
    return t * (((seq_t)6 * t) + (seq_t)5) + (seq_t)1;
}
