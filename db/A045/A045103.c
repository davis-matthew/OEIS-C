/*
 * A045103  Numbers whose base-4 representation contains exactly three 1's and
 *          three 2's.
 * https://oeis.org/A045103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base‑4 representation contains
 *            exactly three 1's and exactly three 2's (computed by
 *            straightforward search)
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Returns true iff x has exactly three digits 1 and exactly three digits 2
   in its base‑4 expansion.  Leading zeros are ignored. */
static inline bool matches(seq_t x)
{
    int c1 = 0, c2 = 0;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)4;
        if (d == (seq_t)1) {
            ++c1;
            if (c1 > 3) return false;
        } else if (d == (seq_t)2) {
            ++c2;
            if (c2 > 3) return false;
        }
        x = x / (seq_t)4;
    }
    return c1 == 3 && c2 == 3;
}

/* Returns the n‑th term of OEIS A045103 (1‑based).  If n == 0 the function
   returns 0, which is never used.  The function scans integers in ascending
   order until the n‑th matching number is found. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    idx_t found = 0;
    for (seq_t v = (seq_t)1; ; ++v) {
        if (matches(v)) {
            ++found;
            if (found == n) return v;
        }
    }
}
