/*
 * A093212  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 12.
 * https://oeis.org/A093212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   a(n) = (for n ≥ 2) concatenation of (n–2) 9's, then “72”, then
 *            (n–2) 0's, then “48”, then (n–2) 0's, then “4”. For n = 1, a(1)
 *            = 0. Each digit is appended by multiplying by 10 and adding the
 *            digit
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)               /* offset is 1 */
        return 0;

    seq_t result = 0;
    idx_t times9 = n - 2;    /* number of leading 9’s and groups of zeros */

    /* leading 9’s */
    for (idx_t i = 0; i < times9; ++i)
        result = result * 10 + 9;

    /* append “72” */
    result = result * 10 + 7;
    result = result * 10 + 2;

    /* first group of zeros */
    for (idx_t i = 0; i < times9; ++i)
        result = result * 10;   /* appending a zero digit */

    /* append “48” */
    result = result * 10 + 4;
    result = result * 10 + 8;

    /* second group of zeros */
    for (idx_t i = 0; i < times9; ++i)
        result = result * 10;

    /* final “4” */
    result = result * 10 + 4;

    return result;
}
