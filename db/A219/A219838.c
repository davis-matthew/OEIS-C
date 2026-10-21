/*
 * A219838  Number of ways to write n as x + y with 0 < x <= y and (xy)^2 + xy
 *          + 1 prime.
 * https://oeis.org/A219838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the pairs (x,y) with 0 < x ≤ y, x + y = n, such that
 *            (x*y)^2 + x*y + 1 is prime
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1881, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t half = n / 2;               /* maximum x with x ≤ y */

    for (idx_t x = 1; x <= half; ++x)
    {
        idx_t y      = n - x;         /* 1 ≤ x ≤ y automatically */
        seq_t sx     = (seq_t)x;      /* cast to seq_t for multiplication */
        seq_t sy     = (seq_t)y;
        seq_t m      = sx * sy;       /* m = x*y */
        seq_t cand   = m * m + m + 1; /* (xy)^2 + xy + 1 */

        if (isPrimeGeneric(cand))
            ++count;
    }
    return count;
}
