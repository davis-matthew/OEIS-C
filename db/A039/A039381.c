/*
 * A039381  Numbers whose base-9 representation has the same nonzero number of
 *          3's and 7's.
 * https://oeis.org/A039381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9443 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-9 representation has the same nonzero number of 3's and 7's.
   Implementation: brute-force search counting the 3's and 7's in each number. */

#include <stdbool.h>

/* Returns true if val has at least one '3' and one '7' in its base‑9
   representation and the counts of those digits are equal. */
static bool has_equal_nonzero_37(seq_t val)
{
    idx_t cnt3 = 0, cnt7 = 0;
    while (val > 0) {
        seq_t d = val % 9;
        if (d == 3)
            cnt3++;
        else if (d == 7)
            cnt7++;
        val /= 9;
    }
    return (cnt3 == cnt7) && cnt3 > 0;
}

/* Returns the nth member of A039381 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset starts at 1; 0 is unused */
    idx_t found = 0;
    for (seq_t x = 1; ; ++x) {
        if (has_equal_nonzero_37(x)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
