/*
 * A229384  Positive integer solutions y1, x1, y2, x2 to Ljunggren's equation
 *          x^2 + 1 = 2y^4.
 * https://oeis.org/A229384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) enumerates y and x from Ljunggren's equation x^2 + 1 = 2y^4
 *            (positive integer solutions)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Store the two solutions (y1,x1) and (y2,x2) computed once: */
    static seq_t values[4] = {0};
    static int ready = 0;

    if (!ready)
    {
        seq_t x = (seq_t)1;     /* x0 = 1   */
        seq_t z = (seq_t)1;     /* z0 = y^2 = 1   */

        size_t found = 0;
        while (found < 2)
        {
            seq_t y = isqrt_seq(z);
            if (y * y == z)
            {
                values[found * 2]     = y;  /* y   */
                values[found * 2 + 1] = x;  /* x   */
                ++found;
            }

            /* Next solution of the corresponding Pell equation:
               (x + z√2) ← (3 + 2√2)(x + z√2)  */
            seq_t nx = (seq_t)3 * x + (seq_t)4 * z;
            seq_t nz = (seq_t)2 * x + (seq_t)3 * z;

            x = nx;
            z = nz;
        }
        ready = 1;
    }

    if (n >= 1 && n <= 4)
        return values[n - 1];
    return (seq_t)0;
}
