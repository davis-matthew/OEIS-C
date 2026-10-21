/*
 * A052225  (n+1)!*(n+3)-3.
 * https://oeis.org/A052225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)!*(n+3)-3 (offset: 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = n + 1;               /* compute (n+1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= k; ++i) {
        fact *= (seq_t)i;          /* multiply by i */
    }
    seq_t result = fact * (seq_t)(n + 3);  /* (n+1)! * (n+3) */
    result -= 3;                          /* subtract 3 */
    return result;
}
