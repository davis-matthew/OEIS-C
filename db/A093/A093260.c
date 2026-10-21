/*
 * A093260  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 60.
 * https://oeis.org/A093260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the OEIS sequence A093260.
   The n-th term (for n ≥ 2) is obtained by concatenating
   (n−2) copies of the digit 9, followed by a single 6,
   then followed by n zeros.  The value for n = 1 is 0.
   For example:
   n = 4 → 9 9 6 0 0 0 = 9960000
*/
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == (idx_t)1)
        return (seq_t)0;
    seq_t res = (seq_t)0;
    idx_t i;

    /* (n-2) copies of '9' */
    for (i = (idx_t)0; i + (idx_t)2 < n; ++i)
        res = res * (seq_t)10 + (seq_t)9;

    /* one '6' */
    res = res * (seq_t)10 + (seq_t)6;

    /* n copies of '0' */
    for (i = (idx_t)0; i < n; ++i)
        res = res * (seq_t)10;

    return res;
}
