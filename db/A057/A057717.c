/*
 * A057717  The non-powers of 10.
 * https://oeis.org/A057717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0, a(n) = n + 1 + floor( log10( n + 1 + floor(
 *            log10( n + 1 ) ) ) ) (i.e. skip the powers of ten)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   ilog10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ilog10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t np1 = (seq_t)n + 1;                 /* n + 1 */
    seq_t d1  = ilog10(np1);          /* floor(log10(n+1)) */
    seq_t t1  = np1 + d1;                      /* n + 1 + floor(log10(n+1)) */
    seq_t d2  = ilog10(t1);           /* floor(log10(t1)) */
    return np1 + d2;                          /* n + 1 + d2 */
}
