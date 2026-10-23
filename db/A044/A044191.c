/*
 * A044191  Numbers n such that string 0,4 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th positive integer whose octal representation
   contains the substring "04" while the preceding integer does
   not.  (OEIS A044191) */

#include <stdbool.h>

static bool has04(seq_t x)
{
    seq_t prev = 0;
    bool first = true;
    while (x != 0) {
        seq_t digit = x % (seq_t)8;
        x = x / (seq_t)8;
        if (!first && prev == 4 && digit == 0)
            return true;
        prev = digit;
        first = false;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; undefined for n = 0 */
        return 0;
    idx_t  count = 0;
    seq_t  m = 0;

    while (count < n) {
        m = m + 1;
        if (has04(m) && !has04(m - 1))
            ++count;
    }
    return m;
}
