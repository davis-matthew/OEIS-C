/*
 * A032888  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(0)
 *          < d(1) > d(2) < ...
 * https://oeis.org/A032888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the nth integer whose base‑9 digits alternate
 * up‑down starting with d0 < d1 > d2 < … */
#include <stdbool.h>

static bool alt9(seq_t x)
{
    seq_t prev = x % 9;
    x /= 9;
    bool need_greater = true;   /* first comparison: d0 < d1   */
    while (x > 0) {
        seq_t cur = x % 9;
        x /= 9;
        if (need_greater) {
            if (!(cur > prev)) return false;
        } else {
            if (!(cur < prev)) return false;
        }
        prev = cur;
        need_greater = !need_greater;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t cand = 1;
    while (cnt < n) {
        if (alt9(cand)) {
            cnt++;
            if (cnt == n) return cand;
        }
        cand++;
    }
    /* unreachable for n >= 1 */
    return 0;
}
