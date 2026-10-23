/*
 * A198831  Successive records in maximal positive distance d = x^3 - y^2.
 * https://oeis.org/A198831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Record of maximal distance d = x^3 - y^2, where y = floor(sqrt(x^3)) */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 (record at x = 1) */
    if (n == 0) return 0;        /* safety for out‑of‑range calls */
    if (n == 1) return 0;

    seq_t max_d = 0;        /* current maximum record value */
    seq_t d;                /* difference for the current x */
    idx_t count = 1;        /* first record already counted (a(1)) */

    for (idx_t x = 2;; ++x)
    {
        seq_t xs  = (seq_t)x;          /* cast x to the seq_t type */
        seq_t x3  = xs * xs * xs;       /* compute x^3 */
        seq_t s   = floor_sqrt(x3);     /* y = floor(sqrt(x^3)) */
        d = x3 - s * s;                 /* d = x^3 - y^2 */

        if (d > max_d)                 /* new record found */
        {
            max_d = d;
            ++count;
            if (count == n)
                return d;              /* return the nth record */
        }
    }
}
