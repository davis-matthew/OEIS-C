/*
 * A044303  Numbers n such that string 5,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n such that the base‑9 representation of n contains
 * the directed pair 5,7 and the base‑9 representation of n-1 does not. */
#include <stdbool.h>

static bool has57_base9(seq_t v)
{
    /* Return true if base‑9 digits of v contain 5 followed by 7 (most to least). */
    if (v == 0)
        return false;
    seq_t lower = v % 9;   /* least significant digit */
    v /= 9;
    while (v > 0) {
        seq_t higher = v % 9;
        if (higher == 5 && lower == 7)
            return true;
        lower = higher;
        v /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1‑based. Search sequentially for the n‑th qualifying integer. */
    seq_t cur = 1;
    idx_t cnt = 0;
    while (1) {
        if (has57_base9(cur) && !has57_base9(cur - 1)) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;
    }
}
