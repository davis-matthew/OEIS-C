/*
 * A203631  Number of nX1 0..1 arrays with the counts of all possible adjacent
 *          horizontal and vertical pair sum values being within one of each
 *          other.
 * https://oeis.org/A203631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n whose counts of 00,
 *            01/10, and 11 adjacent pairs differ by at most one. The
 *            algorithm finds the three pair-sum counts (s0,s1,s2) that are as
 *            equal as possible, enumerates all assignments of the larger
 *            counts, and for each assignment counts the strings b...
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 40, u64 n <= 76, u128 n <= 146,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

static seq_t count_strings(idx_t s0, idx_t s1, idx_t s2)
{
    /* total number of adjacent pairs is n-1 = s0+s1+s2 */
    idx_t R = s1 + 1;              /* number of runs = R = k0 + k1 */
    seq_t total = 0;

    /* Start with 0 */
    {
        idx_t k0 = (R + 1) / 2;    /* zero runs */
        idx_t k1 = R / 2;          /* one runs  */
        seq_t ways = 1;

        if (k0 == 0) {
            if ((s0 + k0) != 0) ways = 0;   /* invalid if zeros exist */
        } else {
            ways *= binom(s0 + k0 - 1, k0 - 1);
        }
        if (ways != 0) {
            if (k1 == 0) {
                if ((s2 + k1) != 0) ways = 0;
            } else {
                ways *= binom(s2 + k1 - 1, k1 - 1);
            }
        }
        total += ways;
    }

    /* Start with 1 */
    {
        idx_t k0 = R / 2;          /* zero runs */
        idx_t k1 = (R + 1) / 2;    /* one runs  */
        seq_t ways = 1;

        if (k0 == 0) {
            if ((s0 + k0) != 0) ways = 0;
        } else {
            ways *= binom(s0 + k0 - 1, k0 - 1);
        }
        if (ways != 0) {
            if (k1 == 0) {
                if ((s2 + k1) != 0) ways = 0;
            } else {
                ways *= binom(s2 + k1 - 1, k1 - 1);
            }
        }
        total += ways;
    }

    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    idx_t n1 = n - 1;
    idx_t q = n1 / 3;
    idx_t r = n1 % 3;

    idx_t base[3] = { q, q, q };
    seq_t result = 0;

    switch (r) {
        case 0: {                    /* all counts equal to q */
            result += count_strings(base[0], base[1], base[2]);
            break;
        }
        case 1: {                    /* one count is q+1 */
            for (int i = 0; i < 3; ++i) {
                idx_t s[3] = { base[0], base[1], base[2] };
                s[i] += 1;
                result += count_strings(s[0], s[1], s[2]);
            }
            break;
        }
        case 2: {                    /* two counts are q+1 */
            for (int i = 0; i < 3; ++i) {
                for (int j = i + 1; j < 3; ++j) {
                    idx_t s[3] = { base[0], base[1], base[2] };
                    s[i] += 1;
                    s[j] += 1;
                    result += count_strings(s[0], s[1], s[2]);
                }
            }
            break;
        }
    }

    return result;
}
