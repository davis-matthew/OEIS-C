/*
 * A214838  Triangular numbers of the form k^2 + 2.
 * https://oeis.org/A214838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k(n)^2 + 2, where k(n) satisfies the Pell-type equation *
 *            x^2 - 8 k^2 = 17. The sequence of k values is obtained by the
 *            two * recurrence sequences: * (x₀ , k₀) = (5 , 1) generates the
 *            odd-indexed terms, * (x₀ , k₀) = (7 , 2) generates the
 *            even-indexed terms. * Recurrence (for both sequence...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine which seed and how many iterations are needed. */
    idx_t t;
    seq_t x, k;
    if (n & 1) {               /* odd n */
        t = (n + 1) >> 1;
        x = (seq_t)5;
        k = (seq_t)1;
    } else {                   /* even n */
        t = n >> 1;
        x = (seq_t)7;
        k = (seq_t)2;
    }

    /* Iterate the recurrence t-1 times. */
    for (idx_t i = 1; i < t; ++i) {
        seq_t x_next = (seq_t)3 * x + (seq_t)8 * k;
        seq_t k_next = x + (seq_t)3 * k;
        x = x_next;
        k = k_next;
    }

    /* Return k^2 + 2. */
    return k * k + (seq_t)2;
}
