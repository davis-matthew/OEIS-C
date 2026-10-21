/*
 * A052360  Number of characters in the English name of n, including spaces
 *          and hyphens.
 * https://oeis.org/A052360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, nice, easy
 * Formula:   a(n) = number of characters (letters, spaces, hyphens) in the
 *            English full name of n, including spaces and hyphens. The
 *            function expands n into groups of three digits and sums the
 *            corresponding word lengths
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   len_1_99, len_1_999
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/len_1_99.h"
#include "../utils/len_1_999.h"

 

/* Main sequence generator.  */
seq_t a(idx_t n)
{
    /* Special case for zero. */
    if (n == 0)
        return (seq_t)4;  /* "zero" */

    /* Factor n into groups of three digits: units, thousands, millions, … */
    const int MAX_GROUPS = 20;          /* enough for 10^60+ */
    idx_t groups[MAX_GROUPS];
    int n_groups = 0;
    idx_t tmp = n;
    while (tmp && n_groups < MAX_GROUPS) {
        groups[n_groups++] = tmp % 1000;
        tmp /= 1000;
    }

    /* Lengths of the scale words for groups 1..19.  */
    static const uint32_t scale_len[20] = {
        0,    /* unused for 10^0 */
        8,    /* thousand  10^3  */
        7,    /* million   10^6  */
        7,    /* billion   10^9  */
        8,    /* trillion  10^12 */
        10,   /* quadrillion 10^15 */
        10,   /* quintillion 10^18 */
        9,    /* sextillion 10^21 */
        9,    /* septillion 10^24 */
        8,    /* octillion 10^27 */
        9,    /* nonillion 10^30 */
        8,    /* decillion 10^33 */
        0,0,0,0,0,0,0   /* unused beyond 10^33 in this build */
    };

    /* lower_nonzero[i] = true if any group j < i is non‑zero. */
    bool lower_nonzero[MAX_GROUPS];
    lower_nonzero[0] = false;
    for (int i = 1; i < n_groups; ++i)
        lower_nonzero[i] = lower_nonzero[i - 1] || (groups[i - 1] != 0);

    seq_t total = 0;
    for (int i = n_groups - 1; i >= 0; --i) {
        if (groups[i] == 0)
            continue;

        /* Add the words for this group (hundreds, tens, units). */
        total += len_1_999(groups[i]);

        /* Add the scale word for groups >= 1. */
        if (i > 0) {
            total += (seq_t)1 + (seq_t)scale_len[i];   /* space + scale word */
            if (lower_nonzero[i])
                total += (seq_t)1;                    /* space before next group */
        }
    }
    return total;
}
