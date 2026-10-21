/*
 * A216486  a(n) is equal to the rational part (considering of the field
 *          Q(sqrt(13))) of the numbers A(n)/sqrt(13), where we have A(n) =
 *          ((sqrt(13) - 1)/2)*A(n-1) + A(n-2) + ((3-sqrt(13))/2)*A(n-3), with
 *          A(0) = 6, A(1) = sqrt(13) - 1, and A(2) = 11 - sqrt(13).
 * https://oeis.org/A216486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 34, i64 n <= 67, i128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216486
   a(n) = -a(n-1) + 5*a(n-2) + 4*a(n-3) - 6*a(n-4) - 3*a(n-5) + a(n-6)
   Initial values: a(0)=0, a(1)=1, a(2)=-1, a(3)=4,
                    a(4)=-3, a(5)=14
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    static const seq_t init[6] = { (seq_t)0, (seq_t)1, (seq_t)-1,
                                   (seq_t)4, (seq_t)-3, (seq_t)14 };
    if (n < 6)
        return init[n];

    /* Use a rolling window of the last six values. */
    seq_t v0 = init[0], v1 = init[1], v2 = init[2],
          v3 = init[3], v4 = init[4], v5 = init[5];
    seq_t next;
    for (idx_t i = 6; i <= n; ++i) {
        next = -v5 + (seq_t)5 * v4 + (seq_t)4 * v3
               - (seq_t)6 * v2 - (seq_t)3 * v1 + v0;
        v0 = v1; v1 = v2; v2 = v3; v3 = v4; v4 = v5; v5 = next;
    }
    return v5;
}
