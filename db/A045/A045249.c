/*
 * A045249  Numbers whose base-5 representation contains no 1's and exactly
 *          two 4's.
 * https://oeis.org/A045249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Enumeration: numbers whose base‑5 representation contains no 1's
   and exactly two 4's.  The function searches numbers in increasing
   order and returns the nth such number (n ≥ 1). */

static bool dfs_len(idx_t len, idx_t pos, seq_t value, idx_t remaining4,
                    idx_t *remaining, seq_t *result)
{
    if (*remaining == 0) return true;
    if (pos == len) {
        if (remaining4 == 0) {
            (*remaining)--;
            if (*remaining == 0) {
                *result = value;
                return true;
            }
        }
        return false;
    }

    /* digits in ascending order: 0,2,3,4 */
    static const int digits[4] = {0, 2, 3, 4};
    for (int i = 0; i < 4; ++i) {
        int d = digits[i];

        if (pos == 0 && d == 0) continue;          /* no leading zero */
        if (d == 4 && remaining4 == 0) continue;    /* no more 4's needed */

        idx_t rem4 = remaining4 - (d == 4 ? 1 : 0);
        idx_t positions_left = len - pos - 1;
        if (rem4 > positions_left) continue;        /* cannot place all 4's */

        seq_t next = value * (seq_t)5 + (seq_t)d;
        if (dfs_len(len, pos + 1, next, rem4, remaining, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 as placeholder */

    idx_t remaining = n;
    seq_t result = 0;

    for (idx_t len = 2; ; ++len) {
        if (dfs_len(len, 0, (seq_t)0, (idx_t)2, &remaining, &result))
            return result;
    }
}
