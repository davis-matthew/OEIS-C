/*
 * A192263  a(n) = abs(a(n-1) - 3*a(n-2)) with a(1)=a(2)=1.
 * https://oeis.org/A192263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(a(n-1) – 3*a(n-2)), a(1) = a(2) = 1, n ≥ 1
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 98, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases */
    if (n <= 2)
        return (seq_t)1;

    /* Iterate the recurrence, keeping only the last two values.
       No dynamic allocation or caching is needed for correctness
       or for the compiler to handle arbitrary‐precision seq_t. */
    seq_t prev2 = (seq_t)1;   /* a(n-2)  */
    seq_t prev1 = (seq_t)1;   /* a(n-1)  */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* 3*a(n-2) */
        seq_t prod = (seq_t)3 * prev2;

        /* absolute difference without using a signed abs to avoid
           possible overflow in the subtraction when seq_t is an
           unsigned type.  */
        if (prev1 >= prod)
            cur = prev1 - prod;
        else
            cur = prod - prev1;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* a(n) is stored in prev1 after the loop */
}
