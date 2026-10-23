/*
 * A151955  a(1) = 102; thereafter a(n) = (a(n-1) with digits sorted into
 *          descending order) - (a(n-1) with digits sorted into ascending
 *          order) (see the Kaprekar map, A151949).
 * https://oeis.org/A151955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 102; thereafter a(n) = (a(n-1) with digits sorted into
 *            descending order) - (a(n-1) with digits sorted into ascending
 *            order) (Kaprekar map, A151949)
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   kaprekar_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/kaprekar_next.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* out of bounds */
    if (n == 1) return (seq_t)102;           /* first term */

    seq_t cur = 102;                         /* a(1) */
    /* Enumerate until n or until the fixed point 495 is reached */
    for (idx_t i = 2; i <= n && cur != (seq_t)495; ++i) {
        cur = kaprekar_next(cur);
    }
    return cur;
}
