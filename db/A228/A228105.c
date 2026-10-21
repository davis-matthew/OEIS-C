/*
 * A228105  a(n) = 432*n^6.
 * https://oeis.org/A228105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 432*n^6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 14, u64 n <= 591, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* 432*0^6 is 0 */
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n^2 */
    seq_t quad = sq * sq;        /* n^4 */
    seq_t quint = quad * nn;     /* n^5 */
    seq_t sext = quint * nn;     /* n^6 */
    return ((seq_t)432) * sext;   /* 432 * n^6 */
}
