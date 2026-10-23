/*
 * A039148  Numbers whose base-10 representation has the same number of 5's
 *          and 8's.
 * https://oeis.org/A039148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-10 representation has the same number of 5's and 8's.
   Computation:   Scan natural numbers upwards, counting 5's and 8's via digit_counts;
                  return the n-th number with equal counts. */

/* Needed helper: digit_counts(seq_t x, unsigned char cnt[10]) is provided by the build system. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"

/* Helper: true if the number has the same number of 5's and 8's. */
static int equal_fives_and_eights(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    return cnt[5] == cnt[8];
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset 1 – values for n<1 are undefined */

    idx_t found = 0;                         /* how many qualifying numbers we have seen */
    seq_t x = 0;                             /* current integer to test */

    while (1)                                 /* loop until the n-th qualifying number is found */
    {
        if (equal_fives_and_eights(x))
        {
            ++found;
            if (found == n)
                return x;
        }
        ++x;                                 /* move to the next integer */
    }
}
