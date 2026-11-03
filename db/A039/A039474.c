/*
 * A039474  Numbers whose base-11 representation has the same nonzero number
 *          of 4's and 7's.
 * https://oeis.org/A039474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base-11 representation contains
 *            the same non‑zero number of digits 4 and 7. a(1)=51. The
 *            function scans successive integers starting from 0, counts the
 *            4’s and 7’s in base 11, and returns the n‑th number that
 *            satisfies the equality
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: test whether x has equal non‑zero counts of 4 and 7 in base‑11. */
static int has_equal_4_7_base11(seq_t x)
{
    seq_t t = x;
    int cnt4 = 0;
    int cnt7 = 0;
    while (t > 0) {
        int d = (int)(t % 11);
        if (d == 4) cnt4++;
        else if (d == 7) cnt7++;
        t /= 11;
    }
    return (cnt4 == cnt7 && cnt4 > 0) ? 1 : 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cur = 0;

    while (1) {
        if (has_equal_4_7_base11(cur)) {
            ++found;
            if (found == n)
                return cur;
        }
        ++cur;
    }
}
