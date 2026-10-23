/*
 * A044077  Numbers n such that string 0,0 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044077: numbers n such that the base‑5 representation of
   n contains the substring "00" while that of n-1 does not.
   This implementation searches for the n‑th such number by brute force. */

#include <stdbool.h>

static bool has00(seq_t x)
{
    seq_t prev = 0;
    bool first = true;
    while (x > 0) {
        seq_t d = x % 5;
        if (!first && prev == 0 && d == 0)
            return true;
        first = false;
        prev = d;
        x /= 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    while (count < n) {
        if (has00(candidate) && !has00(candidate - 1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
    /* Should never reach here for n ≥ 1 */
    return 0;
}
