/*
 * A026565  a(n) = 6*a(n-2), starting with 1, 3, 9.
 * https://oeis.org/A026565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026565
   Recurrence: a(0)=1, a(1)=3.
                a(n)=3*a(n-1) if n is even,
                a(n)=2*a(n-1) if n is odd      (n≥2) */

#include <stddef.h>    /* for size_t, not strictly needed but harmless */

seq_t a(idx_t n)
{
    /* handle the initial two values */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;

    /* start from a(1) and generate onwards */
    seq_t val = (seq_t)3;           /* a(1) */
    for (idx_t i = 2U; i <= n; ++i) {
        if ((i & 1U) == 0U)          /* even index */
            val = (seq_t)3 * val;
        else                         /* odd index  */
            val = (seq_t)2 * val;
    }
    return val;
}
