/*
 * A085857  Number of 3's in decimal expansion of Fibonacci(n).
 * https://oeis.org/A085857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal digit ‘3’ in Fibonacci(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine uses only addition on decimal strings, so it works
   for all builds (fixed‑width seq_t or arbitrary‑precision seq_t).
   No overflow can occur because we never store the Fibonacci number
   in a seq_t until the very end.  The routine is O(n·d), where d
   is the number of decimal digits of F(n) (≈n·log₁₀φ). */

#include <stddef.h>
#include <string.h>
#include "../utils/add_digits.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1, F(n) = 0 or 1 – no ‘3’ digits. */
    if (n == 0 || n == 1) {
        return (seq_t)0;
    }

    /* We store numbers as arrays of decimal digits, least
       significant digit first.  6000 digits are plenty for
       Fibonacci(10000) (~2090 digits). */
    unsigned char A[6000], B[6000], C[6000];
    int lenA = 1, lenB = 1;          /* Fib(0) = 0, Fib(1) = 1 */
    A[0] = 0;
    B[0] = 1;

    int last_count = 0;              /* number of ‘3’ digits in current Fib */

    for (int i = 2; i <= n; ++i) {
        int lenC;
        add_digits(A, lenA, B, lenB, C, &lenC);

        /* Count the digit '3' in C. */
        int count3 = 0;
        for (int j = 0; j < lenC; ++j) {
            if (C[j] == 3) {
                ++count3;
            }
        }
        last_count = count3;

        /* Rotate buffers: A <- B, B <- C */
        memcpy(A, B, lenB * sizeof(unsigned char));
        lenA = lenB;
        memcpy(B, C, lenC * sizeof(unsigned char));
        lenB = lenC;
    }

    return (seq_t)last_count;
}
