/*
 * A092598  Natural numbers n for which sum of decimal digits is greater than
 *          n/4.
 * https://oeis.org/A092598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092598: a(n) is the nth positive integer for which the sum of decimal
 * digits is greater than n/4.  The sequence is finite; its largest term is 49. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t i = 1;            /* candidate integer */
    idx_t cnt = 0;          /* how many qualifying integers have been found */

    while (cnt < n) {
        /* compute digit sum of i */
        seq_t sum = 0;
        idx_t t = i;
        while (t) {
            sum += (seq_t)(t % 10);
            t /= 10;
        }

        /* test the condition digitSum(i) > i/4 (integer division) */
        if (sum > (seq_t)(i / 4)) {
            ++cnt;
            if (cnt == n) {
                return (seq_t)i;
            }
        }
        ++i;
    }

    /* for n larger than the length of the sequence this function returns 0 */
    return (seq_t)0;
}
