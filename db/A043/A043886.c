/*
 * A043886  Numbers k such that 0 and 1 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that the base‑5 representation
 *            of k contains an adjacent pair 0,1 (in either order) while the
 *            representation of k+1 does not. The function searches linearly
 *            for the desired k
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x (in base 5) has a pair 0,1 or 1,0 adjacent. */
static inline bool has_adjacent_01(seq_t x)
{
    bool prev_ok = false;
    seq_t prev = 0;

    while (x > 0) {
        seq_t d = x % 5;
        if (prev_ok) {
            if ((prev == 0 && d == 1) || (prev == 1 && d == 0))
                return true;
        }
        prev = d;
        prev_ok = true;
        x = x / 5;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t found = 0;
    seq_t k = 0;
    seq_t target = (seq_t)n;      /* cast n to seq_t for comparision */

    while (found < target) {
        ++k;
        if (has_adjacent_01(k) && !has_adjacent_01(k + 1))
            ++found;
    }
    return k;
}
