/*
 * A034099  Fractional part of square root of a(n) starts with digit 3.
 * https://oeis.org/A034099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1079, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the n-th integer m such that the fractional part of sqrt(m)
   begins with digit 3 in base 10.
   Equivalent: for each integer k ≥ 0, let
      L(k) = ceil((k+0.3)^2) = ceil((10k+3)^2 / 100)
      U(k) = floor((k+0.4)^2) = floor((10k+4)^2 / 100)
   then all m ∈ [L(k), U(k)] are in the sequence.  a(n) is the
   n-th smallest of these m.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 as placeholder */

    idx_t count = 0;                /* number of terms generated so far */
    seq_t  result = 0;              /* placeholder for final answer */

    for (idx_t k = 1; ; ++k) {
        /* Compute (10*k+3)^2 and (10*k+4)^2 using seq_t arithmetic */
        seq_t tk   = (seq_t)k;                /* ensure promotion to seq_t */
        seq_t num_low   = 10 * tk + 3;        /* 10*k + 3 */
        seq_t num_high  = 10 * tk + 4;        /* 10*k + 4 */
        seq_t sq_low    = num_low * num_low;
        seq_t sq_high   = num_high * num_high;

        /* Ceil division by 100 for low endpoint, floor division for high */
        seq_t start = (sq_low + 99) / 100;    /* ceil(sq_low/100) */
        seq_t end   =  sq_high / 100;         /* floor(sq_high/100) */

        if (end < start) continue;   /* no numbers for this k */

        for (seq_t m = start; ; ++m) {
            ++count;
            if (count == n) {
                return m;   /* nth term found */
            }
            if (m == end) break;
        }
    }
}
