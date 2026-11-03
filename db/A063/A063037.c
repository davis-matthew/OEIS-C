/*
 * A063037  Numbers without 3 consecutive equal binary digits.
 * https://oeis.org/A063037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer (starting from 0) whose binary
 *            representation * contains no three consecutive equal bits. This
 *            is computed by a simple * linear scan checking each candidate
 * Range:     u8 n <= 88, u16 n <= 4180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9258 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: uses only integer operators and bool; works for all seq_t types. */

#include <stdbool.h>

static bool has_no_three(seq_t x)
{
    /* x = 0 is allowed */
    if (x == 0) return true;

    seq_t prev = x & 1;
    seq_t cnt  = 1;
    x >>= 1;
    while (x != 0) {
        seq_t cur = x & 1;
        if (cur == prev) {
            cnt++;
            if (cnt >= 3) return false;
        } else {
            cnt   = 1;
            prev  = cur;
        }
        x >>= 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* We count from 0 upwards until we have found the n-th valid number. */
    idx_t  found = 0;
    seq_t  x     = 0;

    while (1) {
        if (has_no_three(x)) {
            found++;
            if (found == n) return x;
        }
        x++;
    }
}
