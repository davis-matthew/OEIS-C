/*
 * A095489  a(n) = 48 written in base 16 - n.
 * https://oeis.org/A095489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal interpretation of 48 written in base (16-n)
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute a(n) for non‑negative n
 * the indexing follows the OEIS convention (a(0) is the first term).
 * For n > 15 the base would be <= 0, so we return 0.
 */
seq_t a(idx_t n)
{
    /* base = 16 - n; base must be >= 1 to be defined */
    unsigned base = 16u - (unsigned)n;
    if (base == 0) return (seq_t)0; /* undefined, return 0 */

    /* Unary base 1: representation of 48 is 48 ones */
    if (base == 1)
    {
        seq_t val = (seq_t)0;
        for (idx_t i = 0; i < 48; ++i)
        {
            val = val * (seq_t)10 + (seq_t)1;
        }
        return val;
    }

    /* For 2 <= base <= 15 we convert 48 to that base
     * and then interpret the digit string as a decimal integer. */
    uint64_t m = 48u;          /* value to convert */
    unsigned digits[64];       /* enough for any representation up to base 2 */
    int len = 0;
    while (m > 0)
    {
        digits[len++] = (unsigned)(m % base);
        m /= base;
    }
    /* digits[] holds least‑significant first, reverse to get most‑significant first */
    seq_t result = (seq_t)0;
    for (int i = len - 1; i >= 0; --i)
    {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}
