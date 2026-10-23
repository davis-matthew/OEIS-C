/*
 * A045551  Numbers whose factorial has '04' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer m such that * (m! / 10^{v5(m)})
 *            mod 100 = 4, i.e. m! has the two digits * immediately preceding
 *            its trailing zeros equal to 04
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   has_04_before_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_04_before_zeros.h"

seq_t a(idx_t n)
{
    seq_t val = 1;          /* m! with all trailing zeros removed so far */
    idx_t count = 0;        /* how many valid numbers found */

    for (idx_t m = 1;; ++m) {
        val *= (seq_t)m;
        while (val % 10 == 0) val /= 10;
        if (val >= 100000000) val %= 100000000;  /* keep val bounded */

        if (val % 100 == 4) {         /* last two digits before zeros are 04 */
            ++count;
            if (count == n) {
                return (seq_t)m;      /* we found the n-th such number */
            }
        }
    }
}
