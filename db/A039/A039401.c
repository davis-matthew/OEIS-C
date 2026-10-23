/*
 * A039401  Numbers whose base-10 representation has the same nonzero number
 *          of 0's and 9's.
 * https://oeis.org/A039401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A039401 – Numbers whose base‑10 representation has the same
 *  non‑zero number of 0's and 9's.
 *
 *  Implementation: sequentially test every integer, counting its digits
 *  until the desired term is found.
 */
#include <stdint.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* OEIS offset is 1 – undefined for 0 */
        return 0;

    idx_t found = 0;                 /* how many qualifying numbers seen */
    uint64_t cand = 0;               /* candidate integer to test */

    unsigned char cnt[10];

    while (1) {
        ++cand;                      /* start checking from 1 upward */

        /* reset digit counters */
        for (int d = 0; d < 10; ++d)
            cnt[d] = 0;

        digit_counts((seq_t)cand, cnt);  /* fill cnt with digit frequencies */

        if (cnt[0] > 0 && cnt[0] == cnt[9]) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
