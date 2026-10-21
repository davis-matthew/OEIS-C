/*
 * A204557  Right edge of the triangle A045975.
 * https://oeis.org/A204557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2 - 2*n)/2 for n even (n^3 + 2*n^2 - n)/2 for n
 *            odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ni = (seq_t)n;      /* cast index to sequence type early */
    seq_t n2 = ni * ni;       /* n^2  */
    seq_t n3 = n2 * ni;       /* n^3  */
    seq_t term;
    
    if ((n & 1) == 0) {       /* n even  */
        term = n3 + n2 - (ni << 1);     /* n^3 + n^2 - 2*n */
    } else {                  /* n odd   */
        term = n3 + (n2 << 1) - ni;     /* n^3 + 2*n^2 - n */
    }
    
    return term >> 1;         /* divide by 2 */
}
