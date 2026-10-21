/*
 * A052419  Numbers without 7 as a digit.
 * https://oeis.org/A052419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = replace digits d > 6 by d + 1 in base-9 representation of
 *            (n-1)
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 as a safety, although the sequence starts at n = 1 */
    if (n == 0) return 0;

    seq_t m = (seq_t)(n - 1);
    seq_t result = 0;
    seq_t pow10 = 1;                 /* 10^position */

    while (m > 0)
    {
        seq_t d = m % 9;             /* least significant base-9 digit */
        m /= 9;
        seq_t md = (d < 7) ? d : d + 1;  /* map 7→8, 8→9, else same */
        result = result + md * pow10;
        pow10 = pow10 * 10;
    }
    return result;
}
