/*
 * A092428  Numbers n such that A092255(n) == 2 (mod 3).
 * https://oeis.org/A092428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   base3_only_0or1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A092428
 *  a(n) = the nth positive integer whose base‑3 representation
 *  uses only the digits 0 and 1 and contains at least two 1’s
 *  (i.e. it is not a power of 3).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/base3_only_0or1.h"

seq_t a(idx_t n)
{
    idx_t count = 0;   /* how many valid numbers have been found */
    idx_t cur   = 0;   /* candidate integer, using idx_t for range */

    while (1) {
        cur++;                      /* start testing from 1 upward */

        /* Convert to seq_t before performing arithmetic on digits. */
        seq_t val = (seq_t)cur;

        idx_t ones = 0;
        if (!base3_only_0or1(val, &ones))
            continue;                /* digit 2 present → skip */

        if (ones < 2)                /* single 1 → power of 3 → skip */
            continue;

        /* val is a member of the desired set */
        count++;
        if (count == n)
            return val;              /* nth element found */
    }
}
