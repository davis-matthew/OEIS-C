/*
 * A045294  Numbers whose base-5 representation contains no 3's and exactly
 *          two 4's.
 * https://oeis.org/A045294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute a(n): the n-th positive integer whose base‑5 representation
 * contains no digit 3 and exactly two digits 4.  This implementation
 * enumerates the numbers in increasing order and counts those satisfying
 * the constraint until the n‑th one is found.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t   count = 0;
    seq_t   cur   = 0;

    for (;; ++cur) {
        bool ok   = true;
        idx_t fours = 0;
        seq_t tmp   = cur;

        if (tmp == 0) {
            ok = false;                      /* 0 has no digits 4 */
        } else {
            while (tmp > 0) {
                unsigned d = (unsigned)(tmp % 5);
                if (d == 3) { ok = false; break; }
                if (d == 4) { 
                    ++fours; 
                    if (fours > 2) { ok = false; break; } 
                }
                tmp /= 5;
            }
            if (fours != 2) ok = false;
        }

        if (ok) {
            ++count;
            if (count == n) return cur;
        }
    }
}
