/*
 * A044952  Positive numbers with no two equally numerous base 3 digits.
 * https://oeis.org/A044952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9700 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A044952: positive numbers with no two equally numerous base‑3 digits.
   The function scans integers in increasing order and selects those whose ternary
   representation contains no two digits that occur the same number of times. */
#include <stdbool.h>

static bool no_equal_digit_counts(seq_t x)
{
    unsigned cnt[3] = {0,0,0};
    unsigned seen[3] = {0,0,0};
    seq_t t = x;
    while (t != 0)
    {
        unsigned d = (unsigned)(t % (seq_t)3);
        t /= (seq_t)3;
        ++cnt[d];
        seen[d] = 1;
    }
    if (seen[0] && seen[1] && cnt[0] == cnt[1]) return false;
    if (seen[0] && seen[2] && cnt[0] == cnt[2]) return false;
    if (seen[1] && seen[2] && cnt[1] == cnt[2]) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t found = 0;
    for (idx_t i = 1; ; ++i)
    {
        if (no_equal_digit_counts((seq_t)i))
        {
            ++found;
            if (found == n) return (seq_t)i;
        }
    }
}
