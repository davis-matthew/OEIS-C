/*
 * A039402  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 2's.
 * https://oeis.org/A039402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 7958, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose decimal representation has equal nonzero
   number of 1's and 2's; computed by brute‑force search. */

#include <stdbool.h>
#include "../utils/digit_counts.h"

static bool has_equal_1_2(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    return cnt[1] > 0 && cnt[1] == cnt[2];
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = 12;   /* first possible candidate */

    while (found < n) {
        if (has_equal_1_2(cur)) {
            ++found;
            if (found == n)
                return cur;
        }
        cur += 1;
    }
    return 0;    /* never reached for n ≥ 1 */
}
