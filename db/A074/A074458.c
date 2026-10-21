/*
 * A074458  Number of segments lit to display the number n on a 7-segment
 *          display (as in pocket calculators): variant where '6', '7' and '9'
 *          use 6, 4 resp. 5 segments.
 * https://oeis.org/A074458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of segments lit in a 7‑segment display for the
 *            digits of n. The segment counts per digit are: 0 → 6, 1 → 2, 2 →
 *            5, 3 → 5, 4 → 4, 5 → 5, 6 → 6, 7 → 4, 8 → 7, 9 → 5. For n=0 the
 *            value is 6. The computation is done iteratively in base‑10
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* digit segment counts */
    static const seq_t seg[10] = {6, 2, 5, 5, 4, 5, 6, 4, 7, 5};

    if (n == 0)
        return seg[0];        /* a(0) = 6 */

    seq_t sum = 0;
    while (n > 0) {
        /* n % 10 is an idx_t; cast to index into seg, which is seq_t */
        sum += seg[n % 10];
        n /= 10;
    }
    return sum;
}
