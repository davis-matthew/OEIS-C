/*
 * A064808  a(n) is the (n+1)st (n+2)-gonal number.
 * https://oeis.org/A064808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2 + 2)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* convert index to seq_t */
    seq_t nsq = ns * ns;          /* n^2 */
    seq_t num = (ns + 1) * (nsq + 2); /* (n+1)*(n^2+2) */
    return num / 2;               /* divide by 2 */
}
