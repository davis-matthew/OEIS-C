/*
 * A066279  a(n) = n^n + n + 1.
 * https://oeis.org/A066279
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^n + n + 1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes a(n) = n^n + n + 1 for n ≥ -1.
   For n == -1 the sequence specifies a(-1) = -1.
   All operations use seq_t arithmetic and the index type idx_t. */
seq_t a(idx_t n)
{
    /* Handle the special offset term */
    if (n == -1)
        return (seq_t)-1;

    /* For n == 0, 0^0 is treated as 1 */
    seq_t base = (seq_t)n;
    seq_t pow  = (seq_t)1;

    /* Compute base^n iteratively (O(n) multiplications) */
    for (idx_t i = 0; i < n; ++i)
        pow *= base;

    /* a(n) = n^n + n + 1 */
    return pow + base + (seq_t)1;
}
