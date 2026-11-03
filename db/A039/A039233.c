/*
 * A039233  Numbers whose base-12 representation has the same number of 2's
 *          and 6's.
 * https://oeis.org/A039233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-12 representation has the same number of 2's and 6's. */
/* a(1)=0, a(2)=1, a(3)=3, …, as defined in OEIS A039233. */

#include <stdbool.h>

static bool has_equal_2_and_6(seq_t n)
{
    unsigned count2 = 0, count6 = 0;
    if (n == 0) return true;          /* 0 has 0 twos and 0 sixes */
    seq_t base = 12;
    while (n > 0) {
        unsigned digit = (unsigned)(n % base);
        if (digit == 2) ++count2;
        else if (digit == 6) ++count6;
        n = n / base;
    }
    return count2 == count6;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, but safe */
    idx_t target = n;
    idx_t found  = 0;
    seq_t cur = 0;
    while (found < target) {
        if (has_equal_2_and_6(cur)) ++found;
        if (found == target) return cur;
        ++cur;
    }
    return 0;                            /* unreachable */
}
