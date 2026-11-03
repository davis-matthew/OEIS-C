/*
 * A039244  Numbers whose base-12 representation has the same number of 3's
 *          and 9's.
 * https://oeis.org/A039244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039244: a(n) is the n‑th nonnegative integer whose base-12
 * representation contains an equal number of digits 3 and 9.  */
#include <stdbool.h>

static bool has_equal_3_and_9(seq_t v)
{
    int c3 = 0, c9 = 0;
    seq_t tmp = v;
    const seq_t base = (seq_t)12;
    while (tmp > 0)
    {
        seq_t d = tmp % base;
        if (d == (seq_t)3) {
            ++c3;
        } else if (d == (seq_t)9) {
            ++c9;
        }
        tmp /= base;
    }
    return c3 == c9;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    seq_t val = 1;
    idx_t count = 1;          /* we have already counted 0 */

    while (1)
    {
        if (has_equal_3_and_9(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++val;   /* next integer to test */
    }
}
