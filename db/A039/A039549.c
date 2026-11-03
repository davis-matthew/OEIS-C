/*
 * A039549  Numbers whose base-12 representation has the same nonzero number
 *          of 7's and 8's.
 * https://oeis.org/A039549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this function searches sequentially for the numbers whose
 *            base‑12 * representation contains the same non‑zero number of
 *            digit 7’s and digit 8’s * (and at least one of each). The
 *            sequence is returned in ascending order, * with a(1) = 92
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool has_equal_nonzero_78(seq_t x)
{
    int cnt7 = 0, cnt8 = 0;
    seq_t temp = x;
    while (temp > (seq_t)0) {
        seq_t digit = temp % (seq_t)12;
        temp       = temp / (seq_t)12;
        if (digit == (seq_t)7) cnt7++;
        else if (digit == (seq_t)8) cnt8++;
    }
    return cnt7 && cnt8 && cnt7 == cnt8;
}

seq_t a(idx_t n)
{
    /* n is 1‑based, returns a(n) */
    idx_t found = 0;
    seq_t num   = (seq_t)1;
    while (1) {
        if (has_equal_nonzero_78(num)) {
            found++;
            if (found == n) return num;
        }
        num++;
    }
}
