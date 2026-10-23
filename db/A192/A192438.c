/*
 * A192438  Number of bases <= n in which n has no digits exceeding 9.
 * https://oeis.org/A192438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases b with 1 ≤ b ≤ n such that the base‑b
 *            representation of n has only digits 0–9. Bases 1 and 2–9 are
 *            always valid; bases b ≥ 10 are counted by checking the digit
 *            decomposition of n in base b
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* outside the sequence's offset */
        return 0;

    /* Count base 1 (always valid). */
    seq_t count = 1;

    /* Bases 2 through min(n,9) are always valid. */
    if (n >= 2) {
        idx_t upper = (n < 9 ? n : 9);
        count += (seq_t)(upper - 1);   /* bases 2…upper */
    }

    /* Check bases 10 through n. */
    if (n >= 10) {
        for (idx_t b = 10; b <= n; ++b) {
            idx_t tmp = n;
            bool bad = false;
            while (tmp > 0) {
                idx_t digit = tmp % b;
                if (digit >= 10) {   /* digit exceeds 9 → invalid base */
                    bad = true;
                    break;
                }
                tmp /= b;
            }
            if (!bad)
                ++count;
        }
    }

    return count;
}
