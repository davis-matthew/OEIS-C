/*
 * A044924  a(n) = so - se, where so(se) = sum of odd(even) base-2 run lengths
 *          of n.
 * https://oeis.org/A044924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum_{runs of base-2 representation of n} (run_length if
 *            run_length is odd -run_length if run_length is even) i.e. a(n) =
 *            so - se, where so = sum of odd run lengths, se = sum of even run
 *            lengths
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;   /* result so - se */
    idx_t x = n;     /* work variable, same as n (>=1) */

    while (x != 0) {
        idx_t bit = x & 1;   /* current run bit 0 or 1 */
        idx_t len = 0;       /* run length */

        /* count consecutive bits equal to 'bit' */
        while ((x & 1) == bit) {
            len++;
            x >>= 1;
        }

        if (len & 1)           /* odd length run */
            res += (seq_t)len;
        else                   /* even length run */
            res -= (seq_t)len;
    }

    return res;
}
