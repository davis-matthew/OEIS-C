/*
 * A044995  Numbers whose base-3 representation contains exactly one 0 and one
 *          2.
 * https://oeis.org/A044995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 285, u32 n <= 2488, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_completions, cumulative_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044995: Numbers whose base‑3 representation contains exactly one 0 and one 2. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_completions.h"
#include "../utils/cumulative_count.h"

seq_t a(idx_t n)
{
    /* Find the minimal length L with cumulative_count(L) >= n */
    idx_t L = 1;
    while (cumulative_count(L) < n) ++L;

    idx_t prev = cumulative_count(L - 1);          /* terms before length L */
    idx_t offset = n - prev - 1;                   /* zero‑based offset in this length block */

    idx_t cnt1 = (L - 1) * (L - 2);                /* numbers with leading 1  */
    idx_t cnt2 = (L - 1);                          /* numbers with leading 2  */

    seq_t value = 0;

    if (offset < cnt1) {
        /* Leading digit is 1 */
        value = 1;                                 /* start with leading 1 */
        idx_t remZero = 1, remTwo = 1;

        for (idx_t i = 1; i < L; ++i) {
            idx_t rem = (L - 1) - i;                /* positions still to fill */

            for (int d = 0; d < 3; ++d) {
                if (d == 0 && remZero == 0) continue;
                if (d == 2 && remTwo  == 0) continue;

                idx_t rz1 = remZero - (d == 0);
                idx_t rt1 = remTwo  - (d == 2);

                idx_t ways = count_completions(rem, rz1, rt1);
                if (ways > offset) {
                    value = value * 3 + (seq_t)d;
                    remZero = rz1;
                    remTwo  = rt1;
                    break;
                } else {
                    offset -= ways;
                }
            }
        }
    } else {
        /* Leading digit is 2 */
        idx_t local = offset - cnt1;               /* offset within this group */
        idx_t zeroPos = local;                    /* position (0‑based) of the single 0 among the remaining L‑1 digits */

        value = 2;                                 /* leading 2 */
        for (idx_t i = 0; i < L - 1; ++i) {
            seq_t digit = (i == zeroPos) ? 0 : 1;
            value = value * 3 + digit;
        }
    }

    return value;
}
