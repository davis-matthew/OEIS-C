/*
 * A080837  Jouka numbers: either the number is divisible by 7 or 11 or has a
 *          7 in its decimal representation.
 * https://oeis.org/A080837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth positive integer such that the number is divisible by
 *            7, divisible by 11, or contains a digit 7
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9902 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: return true if x contains the digit 7. */
static inline bool has_seven(seq_t x)
{
    while (x)
    {
        if ((x % (seq_t)10) == (seq_t)7)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n==0, but return 0 */

    idx_t count = 0;
    idx_t candidate = 1;

    while (true)
    {
        if ((candidate % 7 == 0) || (candidate % 11 == 0) || has_seven((seq_t)candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
