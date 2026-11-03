/*
 * A044671  Numbers n such that string 4,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑9
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// representation contains the substring "43" while the
// representation of n+1 does NOT contain "43".

#include <stdbool.h>

/* Check whether the base‑9 digits of x contain the adjacent pair 4,3
 * (i.e. a digit 3 followed by a higher‑place digit 4). */
static bool has_substring_43(seq_t x)
{
    seq_t prev = (seq_t)9;           // sentinel value > 8
    seq_t val  = x;
    while (val > (seq_t)0) {
        seq_t cur = val % (seq_t)9;  // least significant digit
        if (prev == (seq_t)3 && cur == (seq_t)4)
            return true;
        prev = cur;
        val /= (seq_t)9;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but guard against 0 */

    idx_t found = 0;          /* number of terms found so far */
    seq_t num = (seq_t)0;     /* start from 1 */

    for (;;) {
        ++num;                         /* candidate number */

        if (has_substring_43(num) && !has_substring_43(num + (seq_t)1)) {
            ++found;
            if (found == n)
                return num;
        }
    }
}
