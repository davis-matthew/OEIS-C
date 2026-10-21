/*
 * A228874  a(n) = L(n) * L(n+1) * L(n+2) * L(n+3), the product of four
 *          consecutive Lucas numbers, A000032.
 * https://oeis.org/A228874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n) * L(n+1) * L(n+2) * L(n+3) where L is the Lucas
 *            sequence L(0)=2, L(1)=1. Computation uses an O(n) iterative
 *            algorithm and does not require any dynamic memory allocation
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Lucas numbers are generated iteratively.
       prev2 holds L(i-2), prev1 holds L(i-1), curr holds L(i). */
    seq_t prev2 = 2;   /* L(0)  */
    seq_t prev1 = 1;   /* L(1)  */
    seq_t curr;

    /* Variables to store the four required numbers. */
    seq_t Ln_val, Ln1_val, Ln2_val, Ln3_val;

    /* Iterate from i = 0 up to i = n+3 inclusive. */
    for (size_t i = 0; i <= (size_t)n + 3; ++i) {
        if (i == 0) {
            curr = prev2;
        } else if (i == 1) {
            curr = prev1;
        } else {
            curr = prev1 + prev2;   /* L(i) = L(i-1) + L(i-2) */
            prev2 = prev1;
            prev1 = curr;
        }

        /* Capture the four consecutive Lucas numbers. */
        if (i == n)      Ln_val  = curr;
        if (i == n + 1)  Ln1_val = curr;
        if (i == n + 2)  Ln2_val = curr;
        if (i == n + 3)  Ln3_val = curr;
    }

    return Ln_val * Ln1_val * Ln2_val * Ln3_val;
}
