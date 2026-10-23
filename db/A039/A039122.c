/*
 * A039122  Numbers whose base-10 representation has the same number of 1's
 *          and 4's.
 * https://oeis.org/A039122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th nonnegative integer whose decimal representation
 *            contains an equal number of digit 1’s and digit 4’s. Computed by
 *            a simple search over the integers
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t and idx_t */
#include <stdint.h>   /* for uint64_t (used by some helper types) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* n≥1 for OEIS, but guard anyway */

    idx_t found = 0;                     /* number of matches seen */
    seq_t cand = 0;                      /* candidate integer */

    for ( ; found < n ; ++cand ) {
        int c1 = 0, c4 = 0;
        seq_t temp = cand;

        /* Count 1’s and 4’s in the decimal representation of cand */
        if (temp != 0) {
            while (temp > 0) {
                int d = (int)(temp % 10);
                if (d == 1) ++c1;
                else if (d == 4) ++c4;
                temp /= 10;
            }
        }
        /* 0 has 0 of each digit, so it satisfies the condition */

        if (c1 == c4) {
            ++found;
            if (found == n) return cand;
        }
    }

    return cand;   /* unreachable, but keeps the compiler happy */
}
