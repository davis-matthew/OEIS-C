/*
 * A075476  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of form 64n+7. Corresponds to selection of every 16th term
 *          from A074474.
 * https://oeis.org/A075476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the Collatz sequence starting at 64*n+7 until
 *            the first term strictly less than 64*n+7, counting the final
 *            term. That is, the number of terms visited (initial term plus
 *            all subsequent values) up to and including the first value <
 *            64*n+7
 * Range:     u8 n <= 0, u16 n <= 36, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t start = (seq_t)64 * n + (seq_t)7;
    seq_t x = start;
    seq_t count = 1;          /* count the initial term */

    while (x >= start) {
        /* next Collatz step */
        if ((x & (seq_t)1) == 0)     /* even */
            x = x / (seq_t)2;
        else                        /* odd */
            x = (seq_t)3 * x + (seq_t)1;
        count++;                    /* count this new term */
    }
    return count;
}
