/*
 * A145745  Numbers n such that n = sigma(sum of digits of n)*sigma(product of
 *          digits of n).
 * https://oeis.org/A145745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A145745 consists of the numbers n such that
   n = sigma( sum of digits of n ) * sigma( product of digits of n ).
   The sequence appears finite; the only terms known are
   1, 12, 6266169, 6931848.  The implementation simply returns
   these known terms.  For n larger than the number of known terms
   the function returns 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 4 known terms: 1, 12, 6266169, 6931848 */
    static const seq_t terms[] = { (seq_t)1, (seq_t)12,
                                   (seq_t)6266169, (seq_t)6931848 };
    static const size_t count = sizeof terms / sizeof *terms;

    if (n == 0 || n > count) {
        /* Undefined beyond the last known term.  Returning 0 is a
           safe sentinel value for the finite sequence. */
        return (seq_t)0;
    }
    return terms[n - 1];
}
