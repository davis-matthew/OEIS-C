/*
 * A140142  a(1)=1, a(n)=a(n-1)+n^0 if n odd, a(n)=a(n-1)+ n^4 if n is even.
 * https://oeis.org/A140142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a(n-1)+1 if n odd, a(n-1)+n^4 if n even
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* count of odd integers up to n */
    seq_t oddcnt = (seq_t)(n + 1) / 2;

    /* m = floor(n/2) */
    idx_t m0 = n / 2;
    seq_t m = (seq_t)m0;

    /* sum of fourth powers 1^4+2^4+...+m^4 */
    seq_t sum4 = m * (m + 1) * (2 * m + 1) *
                 (3 * m * m + 3 * m - 1) / 30;

    return oddcnt + 16 * sum4;
}
