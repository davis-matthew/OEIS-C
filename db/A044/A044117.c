/*
 * A044117  Numbers n such that string 2,3 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer n ≥ 1 such that the base‑6 digits of n
 *            contain the substring "23" while the base‑6 digits of n−1 do
 *            not. The function iteratively scans natural numbers, checks this
 *            property, and returns the n-th such value. All arithmetic is
 *            performed in the input type idx_t and the resu...
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 914 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_23_in_base6(idx_t v)
{
    if (v == 0) return false;          /* base‑6 0 has no digits 2·6^k+3 */
    int prev = -1;
    while (v > 0) {
        int d = v % 6;                  /* least significant digit */
        if (prev >= 0 && prev == 3 && d == 2)
            return true;                /* 2 followed by 3 in normal order */
        prev = d;
        v /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 1;

    while (1) {
        if (has_23_in_base6(num)) {
            if (!has_23_in_base6(num - 1)) {
                ++count;
                if (count == n)
                    return (seq_t)num;
            }
        }
        ++num;
    }
}
