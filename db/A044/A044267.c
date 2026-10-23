/*
 * A044267  Numbers n such that string 1,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044267: Return the n‑th integer n such that the base‑9 representation
 * of n contains the substring "17", but the representation of n–1 does
 * not. */
#include <stdbool.h>

static bool has17(seq_t x)
{
    if (x == 0) return false;
    /* least significant digit */
    seq_t prev = x % 9;
    x = x / 9;
    while (x != 0) {
        seq_t cur = x % 9;  /* more significant digit */
        if (cur == 1 && prev == 7) return true;
        prev = cur;
        x = x / 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t m = 1;
    while (1) {
        if (has17(m)) {
            if (!has17(m - 1)) {
                ++seen;
                if (seen == n) return m;
            }
        }
        ++m;
    }
}
