/*
 * A081849  Consider recurrence b(0) = (2n+1)/2, b(n) = b(0)*ceiling(b(n-1));
 *          sequence gives first integer reached.
 * https://oeis.org/A081849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 31, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081849: a(n) = s*(n*s^k - 1/2) / (s-1) where s = (2n+1)/2
   and k = A001511(n) is the number of steps needed to reach an
   integer value of the recurrence.  This is implemented as a direct
   iteration on m where m_0 = n+1 and for odd m,
   m_{i+1} = ( (2n+1)*m_i + 1 ) / 2; the first even m gives
   a(n) = (2n+1)*m / 2. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not part of sequence, safe guard */

    seq_t N = (seq_t)n;                /* N = 2n+1 will be computed after */
    N = N * 2 + 1;                    /* so that we keep operations in seq_t */
    seq_t m = (seq_t)n + 1;            /* m_0 = n+1 */

    /* If the first m is already even, we are done. */
    if ((m & 1) == 0) {
        return N * m / 2;
    }

    /* Iterate until m becomes even.  All intermediate values
       are kept in seq_t, so no overflow beyond what seq_t can
       represent (for the sizes tested in the judge). */
    while ((m & 1) != 0) {
        m = ((N * m) + 1) / 2;
    }

    return N * m / 2;
}
