/*
 * A039437  Numbers whose base-10 representation has the same nonzero number
 *          of 8's and 9's.
 * https://oeis.org/A039437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: numbers whose base‑10 representation contains the same
 * non‑zero number of 8’s and 9’s.  Implementation: brute‑force search
 * that scans positive integers until the n‑th qualifying number is
 * found.  This works for any build (fixed‑width or arbitrary‑precision)
 * because it only uses integer arithmetic and digit counting.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* offset starts at 1; behaviour for 0 is irrelevant */

    idx_t found = 0;           /* how many terms we have seen */
    seq_t x = 1;               /* candidate number */

    while (1)
    {
        /* Count decimal digits 0–9 in x */
        unsigned char cnt[10];
        digit_counts(x, cnt);

        /* check condition: non‑zero count of 8's and 9's and equal */
        if (cnt[8] > 0 && cnt[8] == cnt[9])
        {
            ++found;
            if (found == n)
                return x;
        }
        ++x;   /* move on to next integer */
    }
}
