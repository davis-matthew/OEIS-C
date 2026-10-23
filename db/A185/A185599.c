/*
 * A185599  a(n) = floor(n^(3/2) - n^(1/2)) - n; complement of A185600.
 * https://oeis.org/A185599
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2) - n^(1/2)) - n
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 80, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation is done by observing that 
   (n-1)*sqrt(n) = sqrt((n-1)^2 * n). 
   Hence a(n) = floor(sqrt((n-1)^2 * n)) - n. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 3) /* offset is 3; behaviour below offset is irrelevant */ 
        return (seq_t)0;

    seq_t N  = (seq_t)n;   /* convert index to seq_t */
    seq_t n1 = N - 1;      /* n-1 */

    /* compute (n-1)^2 * n in seq_t arithmetic   */
    seq_t temp = n1 * n1 * N;

    /* floor(sqrt(temp)) gives floor((n-1)*sqrt(n)) */
    seq_t s = floor_sqrt(temp);

    return s - N;
}
