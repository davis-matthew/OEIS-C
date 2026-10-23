/*
 * A032886  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(0)
 *          < d(1) > d(2) < ...
 * https://oeis.org/A032886
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

/* Computes A032886: numbers whose base‑7 digits alternate
   d0 < d1 > d2 < d3 > … starting with '<' */
#include <stdbool.h>

static bool zigzag_base7(seq_t v)
{
    if (v == 0) return false;
    seq_t prev = v % 7;
    v /= 7;
    bool expect_less = true;
    while (v > 0) {
        seq_t cur = v % 7;
        if (expect_less) {
            if (!(prev < cur)) return false;
        } else {
            if (!(prev > cur)) return false;
        }
        prev = cur;
        expect_less = !expect_less;
        v /= 7;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 1;
    while (true) {
        if (zigzag_base7(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
