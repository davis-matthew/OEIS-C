/*
 * A078199  Smallest multiple of n (for n >= 100) in which frequency of each
 *          occurring digit is the same.
 * https://oeis.org/A078199
 *
 * Offset:    100    (a(100) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k≥1 such that (k·n) has the same frequency for
 *            every decimal digit that actually appears in k·n. The
 *            implementation simply searches multiples of n until the
 *            condition is met
 * Range:     u8 none, u16 n <= 999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   find_min_multiple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/find_min_multiple.h"

/* External function required by the OEIS framework.  For indices below
   the offset (100) the behavior is irrelevant, so we simply return 0. */
seq_t a(idx_t n)
{
    if (n < 100)          /* offset: n = 100 is the first valid term */
        return (seq_t)0;
    return find_min_multiple(n);
}
