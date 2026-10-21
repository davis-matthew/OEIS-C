/*
 * A064693  Number of connected components remaining when decimal expansion of
 *          the number n is cut from a piece of paper.
 * https://oeis.org/A064693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 + (number of holes in the decimal representation of n)
 *            where a hole is present for digits 0,4,6,9 (1 hole each) and
 *            digit 8 (2 holes). All other digits contribute 0 holes. (a(0)=2
 *            follows from 0 having one hole.)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Mapping of decimal digits to number of holes. */
static const int holes[10] = {
    1, /* 0 */
    0, /* 1 */
    0, /* 2 */
    0, /* 3 */
    1, /* 4 */
    0, /* 5 */
    1, /* 6 */
    0, /* 7 */
    2, /* 8 */
    1  /* 9 */
};

seq_t a(idx_t n) {
    /* We count holes digit by digit. */
    uint64_t m = (uint64_t)n;   /* n fits in uint64_t */
    seq_t hole_sum = 0;

    if (m == 0) {
        hole_sum = holes[0];
    } else {
        while (m > 0) {
            int d = (int)(m % 10ULL);
            hole_sum += holes[d];
            m /= 10ULL;
        }
    }

    /* The number of connected components is holes + 1. */
    return hole_sum + 1;
}
