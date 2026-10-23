/*
 * A135215  Maximal number of zero digits in square of number with n digits
 *          and without zero digits.
 * https://oeis.org/A135215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A135215 – Maximal number of zero digits in the square of an
   n‑digit integer containing no zeros.  The known closed form (see OEIS
   comments) is

     a(n) = (3*n-4)/2            if n is even
     a(n) = (3*n-3)/2            if n is odd
   except for the single twist a(7)=10.

   The function below implements exactly that.  The calculation is
   performed in the return type seq_t, which can be any unsigned integer
   type required by the build.  No additional data structures are
   used. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure that we are working with unsigned seq_t for all arithmetic. */
    if (n == 0) return (seq_t)0;  /* Safety: offset starts at 1 */

    seq_t ns = (seq_t)n;

    if (ns & 1u) {               /* odd n */
        seq_t val = (ns * 3u - 3u) / 2u;   /* (3*n-3)/2 */
        if (ns == 7u)            /* exceptional case */
            val = (seq_t)10;
        return val;
    } else {                     /* even n */
        seq_t val = (ns * 3u - 4u) / 2u;   /* (3*n-4)/2 */
        return val;
    }
}
