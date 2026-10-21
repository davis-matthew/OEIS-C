/*
 * A220779  Exponent of highest power of 2 dividing the sum 1^n + 2^n + ... +
 *          n^n.
 * https://oeis.org/A220779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d-1 if n = 2^d * odd , otherwise 2*(d-1) if n+1 = 2^d *
 *            odd (d>0)
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    if (n == 0) /* safeguard for calls below offset, not used by OEIS */
        return (seq_t)0;

    /* compute v2(n) */
    idx_t c = 0;
    idx_t m = n;
    while ((m & 1u) == 0)
    {
        c++;
        m >>= 1u;
    }

    if (c > 0)                  /* n is divisible by 2^c, c>=1 */
        return (seq_t)(c - 1);  /* a(n) = d-1 */

    /* n is odd, so use n+1 */
    idx_t d = 0;
    m = n + 1;
    while ((m & 1u) == 0)
    {
        d++;
        m >>= 1u;
    }
    /* d >= 1 because n+1 is even */
    return (seq_t)(2 * (d - 1));  /* a(n) = 2*(d-1) */
}
