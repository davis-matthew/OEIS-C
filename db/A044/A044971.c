/*
 * A044971  Numbers whose base-3 representation contains exactly one 0 and one
 *          1.
 * https://oeis.org/A044971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑3 representation
 *            contains exactly one 0 and exactly one 1, all other digits being
 *            2
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the length L of the base‑3 word that contains a(n).  For
       words of length L (L >= 2) the count is (L-1)^2. */
    idx_t cumulative = 0;
    idx_t L = 2;               /* minimal length */
    for (;;) {
        idx_t count = (L - 1) * (L - 1);
        if (n <= cumulative + count) break;
        cumulative += count;
        ++L;
    }

    idx_t offset = n - cumulative - 1;   /* 0‑based offset within length L */

    /* Build the word digit by digit and compute its value in base 10. */
    bool placed0 = false, placed1 = false;   /* track whether the 0 or 1
                                                has already been placed */
    seq_t  val = 0;
    /* Leading digit */
    if (offset < L - 1) {                     /* leading digit = 1 */
        val = 1;
        for (idx_t pos = 1; pos < L; ++pos) {
            seq_t d = (pos == (offset + 1)) ? 0 : 2;
            val = val * 3 + d;
        }
    } else {                                  /* leading digit = 2 */
        offset -= (L - 1);
        val = 2;
        for (idx_t pos = 1; pos < L; ++pos) {
            for (int cand = 0; cand <= 2; ++cand) {
                if (cand == 0 && placed0) continue;
                if (cand == 1 && placed1) continue;

                bool newPlaced0 = placed0 || (cand == 0);
                bool newPlaced1 = placed1 || (cand == 1);
                idx_t rem = L - 1 - pos;        /* positions left after this */
                int need0 = newPlaced0 ? 0 : 1;
                int need1 = newPlaced1 ? 0 : 1;
                idx_t ways;

                if (need0 == 0 && need1 == 0) {
                    ways = 1;                   /* all remaining digits fixed to 2 */
                } else if (need0 == 1 && need1 == 1) {
                    if (rem < 2) ways = 0;
                    else ways = rem * (rem - 1); /* place both 0 and 1 */
                } else {
                    ways = rem;                  /* place the single remaining digit */
                }

                if (offset < ways) {
                    val = val * 3 + (seq_t)cand;
                    if (cand == 0) placed0 = true;
                    if (cand == 1) placed1 = true;
                    break;                       /* digit chosen, next position */
                } else {
                    offset -= ways;
                }
            }
        }
    }

    return val;
}
