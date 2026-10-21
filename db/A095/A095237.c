/*
 * A095237  a(1)=1; then for n even, a(n)=(sum of previous terms times n) plus
 *          1, for n odd, a(n)=(sum of previous terms times n) minus 1.
 * https://oeis.org/A095237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095237
 * a(1) = 1
 * For n > 1:
 *   S = sum_{k=1}^{n-1} a(k)
 *   if n is even: a(n) = n * S + 1
 *   if n is odd : a(n) = n * S - 1
 * This implementation computes a(n) in O(n) time by iteratively
 * accumulating the summation. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;     /* cumulative sum of a(k) for k < current i */
    seq_t result = 0;  /* value of a(n) to return */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;          /* a(1) = 1 */
        } else {
            seq_t prod = (seq_t)i * sum;
            if ((i & 1) == 0) /* even i */
                val = prod + 1;
            else              /* odd i */
                val = prod - 1;
        }
        sum += val;          /* update cumulative sum */
        if (i == n)
            result = val;    /* store final term */
    }
    return result;
}
