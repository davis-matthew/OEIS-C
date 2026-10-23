/*
 * A168462  a(n) = n^5*(n^8 + 1)/2.
 * https://oeis.org/A168462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5*(n^8 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 31, u128 n <= 970,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast the index to seq_t */
    if (N == 0) return 0;        /* a(0) = 0 */

    if ((N & 1) == 0) {          /* n is even */
        /* Compute n^5/2 first to keep the numbers small */
        seq_t pow5 = N;
        for (unsigned i = 1; i < 5; ++i) pow5 *= N;   /* n^5 */
        pow5 /= 2;                                   /* n^5 / 2 */

        /* Compute n^8 + 1 */
        seq_t pow8 = N;
        for (unsigned i = 1; i < 8; ++i) pow8 *= N;   /* n^8 */
        pow8 += 1;                                   /* n^8 + 1 */

        return pow5 * pow8;                          /* result */
    } else {                                          /* n is odd */
        /* Compute (n^8 + 1) / 2 first */
        seq_t pow8 = N;
        for (unsigned i = 1; i < 8; ++i) pow8 *= N;   /* n^8 */
        seq_t half = (pow8 + 1) >> 1;                 /* (n^8 + 1)/2 */

        /* Compute n^5 */
        seq_t pow5 = N;
        for (unsigned i = 1; i < 5; ++i) pow5 *= N;   /* n^5 */

        return pow5 * half;                          /* result */
    }
}
