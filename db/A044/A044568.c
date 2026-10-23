/*
 * A044568  Numbers n such that string 0,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n for which the base‑8 representation contains "00" but
 *            n+1 does not
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a base‑8 number contains two consecutive zero digits. */
static bool has_double_zero(seq_t x)
{
    if (x < 8) {                 /* only one digit, impossible to have "00" */
        return false;
    }
    seq_t prev = x % 8;
    x /= 8;
    while (x > 0) {
        seq_t cur = x % 8;
        if (prev == 0 && cur == 0) {
            return true;
        }
        prev = cur;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;                 /* start scanning from 1 */
    while (1) {
        if (has_double_zero(m) && !has_double_zero(m + (seq_t)1)) {
            ++count;
            if (count == n) {
                return m;
            }
        }
        ++m;
    }
}
