/*
 * A176237  Binary expansion of n contains at least one 1-bit at even position
 *          and one 1-bit at odd position.
 * https://oeis.org/A176237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176237: a(n) is the nth positive integer whose binary
 * representation contains at least one 1‑bit at an even position
 * (including position 0) and at least one 1‑bit at an odd position.
 * The implementation checks each integer in increasing order until
 * the nth valid number is found.  This is efficient for n up to about
 * 10 000. */
#include <stdbool.h>

/* Return true iff v has at least one 1‑bit in an even position and
 * at least one 1‑bit in an odd position. */
static bool has_even_and_odd(seq_t v)
{
    bool even = false, odd = false;
    int pos = 0;
    seq_t t = v;
    while (t)
    {
        if ((t & 1) != 0)
        {
            if (pos & 1)
                odd = true;
            else
                even = true;
        }
        t >>= 1;
        pos++;
    }
    return even && odd;
}

/* External linkage required. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 0;          /* start from 1 */
    while (count < n)
    {
        ++cur;
        if (has_even_and_odd(cur))
            ++count;
    }
    return cur;
}
