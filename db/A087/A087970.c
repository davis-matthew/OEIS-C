/*
 * A087970  Maximal term in Collatz-iteration started at 2^n+1.
 * https://oeis.org/A087970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum value in the Collatz sequence starting at 2^n + 1
 *            (OEIS A087970). The function is independent of any external
 *            helpers
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute 2^n + 1, avoiding overflow in seq_t by repeated multiplication. */
    seq_t curr = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        curr = curr * (seq_t)2;
    curr += (seq_t)1;          /* start value: 2^n + 1 */

    seq_t max_val = curr;

    /* Run the Collatz iteration until reaching 1, keeping the largest value seen. */
    while (curr != (seq_t)1)
    {
        if ((curr & (seq_t)1) == 0)          /* even */
            curr = curr / (seq_t)2;
        else                                 /* odd */
            curr = curr * (seq_t)3 + (seq_t)1;

        if (curr > max_val)
            max_val = curr;
    }

    return max_val;
}
