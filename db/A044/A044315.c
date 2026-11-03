/*
 * A044315  Numbers n such that string 7,1 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044315
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

/* Sequence A044315: numbers n such that base‑9 representation of n contains
   the substring “71” but base‑9 representation of n‑1 does not. */

#include <stdbool.h>

static bool has71_base9(seq_t x)
{
    /* Scan digits from least significant to most significant. */
    int current = -1;
    while (x > (seq_t)0) {
        int d = (int)(x % 9);
        if (d == 7 && current == 1) return true;
        current = d;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t cur = (seq_t)0;
    idx_t cnt = 0;

    while (1) {
        cur = cur + (seq_t)1;                /* next integer to test */
        if (has71_base9(cur) && !has71_base9(cur - (seq_t)1)) {
            cnt = cnt + (idx_t)1;
            if (cnt == n) return cur;
        }
    }
}
