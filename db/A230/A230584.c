/*
 * A230584  Either two less than a square or two more than a square.
 * https://oeis.org/A230584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = n^2+2; a(2n) = (n+1)^2-2 for n>=2; base a(1)=2, a(2)=3
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    if (n & (idx_t)1) {                /* odd index → a(2m-1) = m^2+2 */
        seq_t m = ((seq_t)n + (seq_t)1) / (seq_t)2;
        return m * m + (seq_t)2;
    } else {                           /* even index → a(2m) = (m+1)^2-2 */
        seq_t m   = (seq_t)n / (seq_t)2;
        seq_t mp1 = m + (seq_t)1;
        return mp1 * mp1 - (seq_t)2;
    }
}
