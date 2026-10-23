/*
 * A045013  Numbers whose base-3 representation contains exactly four 0's and
 *          four 2's.
 * https://oeis.org/A045013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search: iterate natural numbers, test their base‑3
 * representation for exactly four 0's and four 2's, and return the n‑th
 * matching number (OEIS A045013).
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t target = n;             /* how many matches still needed */

    /* Iterate all positive integers (base‑10) and test each */
    for (seq_t v = 1;; ++v) {
        int cnt0 = 0;
        int cnt2 = 0;
        seq_t tmp = v;

        /* Count digits of v in base 3 */
        while (tmp > 0) {
            int d = (int)(tmp % 3);        /* digit 0,1 or 2 */
            if (d == 0) ++cnt0;
            else if (d == 2) ++cnt2;

            /* Early exit if counts exceed 4 */
            if (cnt0 > 4 || cnt2 > 4) break;

            tmp /= 3;
        }

        /* Check if exactly four 0's and four 2's were found */
        if (cnt0 == 4 && cnt2 == 4) {
            if (--target == 0)
                return v;                 /* n‑th term found */
        }
    }
}
