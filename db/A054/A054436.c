/*
 * A054436  Smallest area of a Pythagorean triangle with n as length of a leg.
 * https://oeis.org/A054436
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A055527(n)/2
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest other leg m of a Pythagorean triple containing n
 * by searching over divisors of n²: for any factor pair d|n², e=n²/d,
 * with (d & e) having the same parity, the corresponding other leg is  
 * m = (e-d)/2.  The first suitable divisor encountered when scanning
 * d from n downwards gives the minimal m.  The area is n*m/2. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    /* convert n to seq_t for arithmetic */
    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;
    for (idx_t d = n; d > 0; --d)
    {
        seq_t dseq = (seq_t)d;
        if (sn2 % dseq != 0) continue;           /* not a divisor of n² */
        seq_t e   = sn2 / dseq;
        seq_t diff = e - dseq;
        if (diff == 0) continue;                 /* would give m = 0   */
        if ((diff & 1) != 0) continue;           /* parity mismatch   */
        seq_t m = diff >> 1;                     /* m = (e-d)/2       */
        return (sn * m) / 2;                     /* area = n*m/2      */
    }
    return 0; /* should never be reached for n ≥ 3 */
}
