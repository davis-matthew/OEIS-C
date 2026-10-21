/*
 * A054057  Numbers of form 41+n^2+n or 41+2*n^2.
 * https://oeis.org/A054057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th element (1‑based) of the sorted union of f₁(k)
 *            = 41 + k·(k + 1) (k ≥ 0) and f₂(k) = 41 + 2·k² (k ≥ 0), with
 *            duplicate values omitted
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i1 = 0, i2 = 0;      /* indices for the two quadratic sequences */
    idx_t count = 0;           /* number of values produced so far */
    seq_t result = 0;          /* value to be returned */

    while (count < n)
    {
        /* compute next candidates from both sequences */
        seq_t val1 = (seq_t)41 + (seq_t)i1 * ((seq_t)i1 + 1);
        seq_t val2 = (seq_t)41 + 2 * (seq_t)i2 * (seq_t)i2;

        if (val1 < val2)
        {
            result = val1;
            i1++;           /* advance first sequence */
        }
        else if (val2 < val1)
        {
            result = val2;
            i2++;           /* advance second sequence */
        }
        else            /* duplicate value */
        {
            result = val1;   /* or val2, they're equal */
            i1++;            /* advance both to avoid repeated output */
            i2++;
        }

        count++;
    }

    return result;
}
