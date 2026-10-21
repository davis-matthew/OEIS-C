/*
 * A096400  Number of equivalence classes of triangles having equal angles of
 *          integral degrees and smallest angle = n.
 * https://oeis.org/A096400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = floor((180 - 3*n)/2) + 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for int64_t */

seq_t a(idx_t n)
{
    /* Compute the numerator 180 - 3*n in a signed type to
       handle possible negatives when n > 60.  */
    int64_t numerator = (int64_t)180 - (int64_t)3 * (int64_t)n;

    /* If the numerator is zero or negative, the sequence ends
       at n = 60.  Return 1 for n = 60, 0 for n > 60. */
    if (numerator <= 0)
        return (numerator == 0) ? (seq_t)1 : (seq_t)0;

    /* For positive numerator, integer division truncates toward zero,
       which is exactly floor for positive numbers.  */
    seq_t half = (seq_t)(numerator / 2);
    return half + (seq_t)1;
}
