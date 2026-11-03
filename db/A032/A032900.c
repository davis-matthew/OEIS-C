/*
 * A032900  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑3 digits alternate:
   d0 <= d1 >= d2 <= d3 >= d4 ...  (OEIS A032900) */
#include <stdbool.h>

static bool alt_base3(seq_t x)
{
    seq_t prev = x % 3;
    x = x / 3;
    if (x == 0) return true;          /* single digit is always valid */

    bool le = true;                    /* we must check prev <= cur first */
    while (x != 0) {
        seq_t cur = x % 3;
        x = x / 3;
        if (le) {
            if (!(prev <= cur)) return false;
        } else {
            if (!(prev >= cur)) return false;
        }
        prev = cur;
        le = !le;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1, so n==0 is unused */
    seq_t cand = 1;
    idx_t count = 0;
    while (true) {
        if (alt_base3(cand)) {
            ++count;
            if (count == n) return cand;
        }
        cand = cand + 1;
    }
}
