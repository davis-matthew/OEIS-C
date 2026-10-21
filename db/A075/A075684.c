/*
 * A075684  For odd numbers 2n-1, the maximum number produced by iterating the
 *          reduced Collatz function R defined as R(k) = (3k+1)/2^r, with r as
 *          large as possible.
 * https://oeis.org/A075684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = maximum value in the reduced Collatz trajectory starting
 *            from the odd integer 2n-1. R(k) = (3k+1)/2^v₂(3k+1), r chosen so
 *            that the result is odd
 * Range:     u8 n <= 13, u16 n <= 351, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is 1‑based (a(1) corresponds to the odd number 1). */
    if (n == 0) return 0;          /* undefined for n=0, but safe return */

    seq_t start = (seq_t)n * 2 - 1; /* 2n-1 as a seq_t */
    seq_t current = start;
    seq_t max_val = start;
    seq_t one = 1;

    while (current != one)          /* iterate until 1 (odd Collatz); already odd */
    {
        seq_t t = current * 3 + 1;  /* 3k + 1 */

        /* Divide by the largest power of 2: keep shifting right while even */
        while ((t & 1) == 0)
            t >>= 1;

        current = t;

        if (current > max_val)
            max_val = current;
    }

    return max_val;
}
