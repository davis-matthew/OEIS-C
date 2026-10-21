/*
 * A060228  Numbers ending in the letter N.
 * https://oeis.org/A060228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) is the n‑th positive integer whose English name ends
   with the letter 'n'.
   A number x is in the sequence iff its last two digits are
   10–19 (except 12) or the units digit is 7. */

#include <stdbool.h>

static inline bool term_ok(seq_t x)
{
    idx_t last = (idx_t)(x % 100);          /* 0 … 99 */
    if (last >= 10 && last <= 19 && last != 12)
        return true;                       /* teens except twelve */
    if (last % 10 == 7)
        return true;                       /* numbers ending with 7 */
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;

    while (true)
    {
        ++x;
        if (term_ok(x))
        {
            ++count;
            if (count == n)
                return x;
        }
    }
}
