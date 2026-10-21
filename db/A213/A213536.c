/*
 * A213536  Cousin prime recurrence sequence: a(1) = 14, and for n > 1, a(n) =
 *          a(n-1) + gcd(n+5, a(n-1)), if n is even, otherwise a(n) = a(n-1) +
 *          gcd(n+1, a(n-1)).
 * https://oeis.org/A213536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 1273, u32 n <= 1273, u64 n <= 1273,
 *            u128 n <= 1273, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: Cousin prime recurrence sequence (OEIS A213536).
   Formula: 
      a(1) = 14;
      for n > 1:
        if n is even:  a(n) = a(n-1) + gcd(n+5, a(n-1));
        if n is odd:   a(n) = a(n-1) + gcd(n+1, a(n-1));
   The implementation iteratively computes the terms up to the requested
   index.  It works for any seq_t (including arbitrary-precision
   integers) and for any idx_t (uint64_t).
*/

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1 */

    seq_t prev = 14;              /* a(1) */

    if (n == 1)
        return prev;

    for (idx_t cur = 2; cur <= n; ++cur) {
        seq_t g;
        if (cur & 1)              /* odd */
            g = gcd((seq_t)(cur + 1), prev);
        else                      /* even */
            g = gcd((seq_t)(cur + 5), prev);
        prev = prev + g;
    }
    return prev;
}
