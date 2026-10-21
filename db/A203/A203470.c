/*
 * A203470  a(n) = Product_{2 <= i < j <= n+1} (i + j).
 * https://oeis.org/A203470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{2 <= i < j <= n+1} (i + j)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence
 * a(1) = 1
 * a(n) = a(n-1) * factorial(2n+1) / factorial(n+2)    (n >= 2)
 * where factorial is computed incrementally, so the whole
 * computation stays O(n).                                          */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset is one – a(1) = 1.  For n < 1 return 0 (undefined). */
    if (n < 1u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;

    /* a holds the current a(k), starting with a(1).                */
    seq_t a_val = (seq_t)1;

    /* h = factorial(2*k-1).  Initially for k=1 this is factorial(1)=1. */
    seq_t h = (seq_t)1;

    /* g = factorial(k+2).  Initially for k=1 this is factorial(3)=6.  */
    seq_t g = (seq_t)1 * (seq_t)2 * (seq_t)3;   /* = 6 */

    /* Iterate k = 2 .. n to build up a(k). */
    for (idx_t k = 2; k <= n; ++k)
    {
        /* Update h to factorial(2*k-1) = previous h * (2*k-2) * (2*k-1) */
        h *= (seq_t)(2 * k - 2);
        h *= (seq_t)(2 * k - 1);

        /* numerator = factorial(2*k+1) = h * (2*k) * (2*k+1) */
        seq_t numerator = h * (seq_t)(2 * k) * (seq_t)(2 * k + 1);

        /* Update g to factorial(k+2) = previous g * (k+2) */
        g *= (seq_t)(k + 2);

        /* Update the sequence value: a(k) = a(k-1) * numerator / g */
        a_val *= numerator;
        a_val /= g;
    }

    return a_val;
}
