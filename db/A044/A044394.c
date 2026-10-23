/*
 * A044394  Numbers n such that string 6,2 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Incrementally scan positive integers until the nth number
   whose decimal notation contains the substring "62" but the previous
   integer does not is found. */
#include <stdbool.h>

static inline bool has62(seq_t x) {
    while (x >= ((seq_t)10)) {
        if ((x % ((seq_t)100)) == ((seq_t)62))
            return true;
        x /= ((seq_t)10);
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t cnt = 0;
    seq_t i = 1;                      /* start from 1 */
    while (1) {
        if (has62(i)) {
            seq_t prev = i;
            --prev;                     /* i ≥ 1, so prev ≥ 0 */
            if (!has62(prev)) {
                ++cnt;
                if (cnt == n)
                    return i;
            }
        }
        ++i;
    }
}
