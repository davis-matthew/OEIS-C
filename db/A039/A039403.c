/*
 * A039403  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 3's.
 * https://oeis.org/A039403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose decimal digits contain the
 *            same non‑zero number of 1’s and 3’s. This is computed by
 *            brute‑force enumeration
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    idx_t candidate = 1;
    idx_t found = 0;
    unsigned char cnt[10];

    for (;;) {
        memset(cnt, 0, sizeof(cnt));
        digit_counts((seq_t)candidate, cnt);
        /* cnt[1] and cnt[3] are the counts of digits 1 and 3. */
        if (cnt[1] == cnt[3] && cnt[1] > 0) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
