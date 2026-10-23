/*
 * A104895  a(0)=0; thereafter a(2n) = -2*a(n), a(2n+1) = 2*a(n) - 1.
 * https://oeis.org/A104895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, nice
 * Formula:   not stated
 * Range:     i8 n <= 255, i16 n <= 8998, i32 n <= 8998, i64 n <= 8998,
 *            i128 n <= 8998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A104895:
   a(0) = 0
   For k ≥ 0 and 0 ≤ j < 2^k:
      a(2^k + j) = a(j) + 2^k   if a(j) < 0
      a(2^k + j) = a(j) - 2^k   otherwise   (i.e. a(j) ≥ 0)
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return 0;

    /* find the largest power of two ≤ n : pow2 = 1 << k */
    idx_t pow2 = 1;
    while ((pow2 << 1) <= n)
        pow2 <<= 1;

    /* j = n - pow2, so 0 ≤ j < pow2   */
    idx_t j = n - pow2;

    /* recursive call for the smaller index j */
    seq_t aj = a(j);

    /* adjust according to the sign of aj */
    if (aj < 0)
        return aj + (seq_t)pow2;   /* a(j) + 2^k */
    else
        return aj - (seq_t)pow2;   /* a(j) - 2^k */
}
