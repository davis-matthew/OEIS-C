/*
 * A014305  Duplicate of A023533.
 * https://oeis.org/A014305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 if n-1 is a tetrahedral number k(k+1)(k+2)/6 for some k
 *            ≥ 0, otherwise 0
 * Range:     i8 n <= 33, i16 n <= 33, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* behavior for n < 1 does not matter */

    seq_t target = (seq_t)n - 1;  /* n-1 must be a tetrahedral number */

    if (target == 0)            /* k = 0 gives tetrahedral number 0 */
        return 1;

    seq_t accum = 0;
    for (idx_t k = 1; ; ++k) {
        /* add the k-th triangular number: k*(k+1)/2 */
        accum += (seq_t)k * (k + 1) / 2;
        if (accum == target)
            return 1;
        if (accum > target)
            break;
    }

    return 0;
}
