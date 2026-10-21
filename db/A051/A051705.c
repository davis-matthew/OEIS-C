/*
 * A051705  Numbers whose Japanese name lacks syllable "ku".
 * https://oeis.org/A051705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: numbers whose Japanese name lacks syllable "ku".
 * Classify by decimal digits: a number is valid iff
 *  - it contains no digit '6', and
 *  - its last digit is not '9'.
 *
 * The function returns the n-th such positive integer
 * (a(1) = 1, a(2) = 2, …).
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                               /* undefined for offset 1 */

    idx_t count    = 0;                                 /* how many valid numbers found */
    idx_t candidate = 0;                                /* current integer to test */

    while (1) {
        candidate++;                                    /* test next positive integer */

        /* Reject numbers ending with 9 */
        if ((candidate % 10) == 9) continue;

        /* Reject numbers containing a 6 anywhere */
        idx_t tmp   = candidate;
        bool ok = true;
        while (tmp > 0) {
            if ((tmp % 10) == 6) { ok = false; break; }
            tmp /= 10;
        }
        if (!ok) continue;

        /* candidate is valid */
        ++count;
        if (count == n) return (seq_t)candidate;
    }
}
