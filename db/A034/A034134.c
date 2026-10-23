/*
 * A034134  Decimal part of cube root of a(n) starts with 8: first term of
 *          runs.
 * https://oeis.org/A034134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil((5*n+4)^3 / 125)
 * Range:     u8 none, u16 n <= 7, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = floor(((5*n+4)^3 + 124) / 125) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* si n may be zero, but the sequence is defined for n >= 1 */
    seq_t k = (seq_t)n;
    k = 5 * k + 4;          /* k = 5*n + 4 */
    seq_t num = k * k * k;  /* (5*n+4)^3 */
    const seq_t denom = 125;
    seq_t q = num / denom;
    if (num % denom != 0)
        q++;               /* ceil division */
    return q;
}
