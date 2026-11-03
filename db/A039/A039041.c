/*
 * A039041  Numbers whose base-7 representation has the same number of 2's and
 *          4's.
 * https://oeis.org/A039041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for numbers whose base‑7 representation has
   the same number of 2’s and 4’s.  The function repeatedly tests consecutive
   integers starting at 0 until the required count of valid numbers is reached
   and returns that number. */

#include <stdbool.h>

static inline bool has_equal_2_and_4_idx(idx_t x)
{
    unsigned count2 = 0, count4 = 0;
    while (x > 0) {
        unsigned digit = x % 7;
        if (digit == 2)    ++count2;
        else if (digit == 4) ++count4;
        x /= 7;
    }
    return count2 == count4;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, but guard for n==0 */
    idx_t  count   = 0;
    idx_t  cand    = 0;        /* use idx_t so we avoid overflow on seq_t */
    while (1) {
        if (has_equal_2_and_4_idx(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}
