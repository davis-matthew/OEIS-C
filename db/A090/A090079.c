/*
 * A090079  In binary expansion of n: reduce contiguous blocks of 0's to 0 and
 *          contiguous blocks of 1's to 1.
 * https://oeis.org/A090079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 340, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = binary value of the run‑length reduced binary expansion of n.
   Example: n = 100 → binary "1100100" → runs [11][00][1][00] → "1010" (=10). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0. */
    if (n == 0) return 0;

    seq_t res = 0;          /* Result built as a binary number. */
    int prev_bit = 0;       /* Previous bit encountered in the run. */
    bool started = false;   /* Have we seen the first ‘1’ yet? */

    /* Examine bits from most significant to least significant. */
    for (int i = 63; i >= 0; --i)
    {
        int bit = (int)((n >> i) & 1);
        if (!started)
        {
            /* The most significant set bit is always 1 for n > 0. */
            started = true;
            prev_bit = bit;
            res = (seq_t)bit;
        }
        else if (bit != prev_bit)
        {
            /* A new run begins – append the new bit. */
            res = (res << 1) | (seq_t)bit;
            prev_bit = bit;
        }
    }

    return res;
}
