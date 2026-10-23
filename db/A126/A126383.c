/*
 * A126383  Number of base 29 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A126383: Number of base‑29 n‑digit strings in which adjacent
   digits differ by at most one.
   The function computes the term a(n) (a(0)=1, a(1)=29) by a simple
   dynamic programme on the 29‑vertex path graph.  For n≥2 it keeps
   the count of strings of each ending digit in an array of length 29
   and iterates the recurrence
       next[d] = cur[d] + (d>0?cur[d-1]:0) + (d<28?cur[d+1]:0).
   The total a(n) is the sum of the final array.  All arithmetic is
   performed on the supplied integer type seq_t, so the implementation
   works for the primitive or arbitrary‑precision cases.
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)29;

    /* Arrays of counts for each digit (0..28) */
    seq_t cur[29], next[29];

    /* Base case: all one‑digit strings contribute 1 to each digit */
    for (int d = 0; d < 29; ++d)
        cur[d] = (seq_t)1;

    /* Build up to length n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int d = 0; d < 29; ++d) {
            seq_t sum = cur[d];
            if (d > 0)
                sum += cur[d - 1];
            if (d < 28)
                sum += cur[d + 1];
            next[d] = sum;
        }
        /* Move next to cur for the next iteration */
        for (int d = 0; d < 29; ++d)
            cur[d] = next[d];
    }

    /* Sum over all ending digits to obtain a(n) */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 29; ++d)
        total += cur[d];

    return total;
}
