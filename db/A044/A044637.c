/*
 * A044637  Numbers n such that string 0,5 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 889, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 983 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044637
 * Numbers n such that the base‑9 representation of n contains the
 * substring "05" but the base‑9 representation of n+1 does not.
 */

#include <stdbool.h>

/* Return true iff base‑9 digits of v contain the adjacent pair 0,5 */
static bool has_05_in_base9(seq_t v)
{
    if (v == 0)
        return false;            /* single digit 0 has no substring */

    int digits[32];              /* enough for 9^31 > 2^128 */
    int len = 0;
    seq_t t = v;
    while (t > 0) {
        digits[len++] = (int)(t % 9);
        t = t / 9;
    }

    /* digits[] holds least‑significant digit first.
       We need a 0 followed by a 5 when read most to least. */
    for (int i = 1; i < len; i++) {
        if (digits[i] == 0 && digits[i - 1] == 5)
            return true;
    }
    return false;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    seq_t cur = 1;      /* start searching from 1 */
    idx_t found = 0;

    while (1) {
        if (has_05_in_base9(cur) && !has_05_in_base9(cur + 1)) {
            found++;
            if (found == n)
                return cur;
        }
        cur++;
    }
}
