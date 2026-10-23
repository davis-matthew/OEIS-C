/*
 * A044069  Numbers n such that string 2,0 occurs in the base 4 representation
 *          of n but not of n-1.
 * https://oeis.org/A044069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A044069: numbers n such that the base‑4
   representation of n contains the substring "20" but the base‑4 representation
   of n-1 does not. */
#include <stdbool.h>

static inline bool contains20(seq_t x)
{
    bool prev_is_zero = false;
    while (x > 0) {
        seq_t d = x % 4;
        if (prev_is_zero && d == 2)
            return true;
        prev_is_zero = (d == 0);
        x /= 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    for (seq_t k = 1; ; ++k) {
        if (contains20(k) && !contains20(k - 1)) {
            ++cnt;
            if (cnt == n)
                return k;
        }
    }
}
