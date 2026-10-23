/*
 * A138166  Numbers containing their length in their decimal representation.
 * https://oeis.org/A138166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138166: numbers whose decimal representation contains the
   digit equal to its length (search method). */

#include <stdbool.h>

/* Count decimal digits of x (x > 0). */
static int digit_len(seq_t x)
{
    int len = 0;
    do
    {
        len++;
        x /= (seq_t)10;
    } while (x > (seq_t)0);
    return len;
}

/* Return true iff x contains the digit d. */
static bool contains_digit(seq_t x, unsigned d)
{
    while (x > (seq_t)0)
    {
        if ((x % (seq_t)10) == (seq_t)d)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

/* Return the n-th number whose decimal representation contains its length. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 1;

    while (1)
    {
        seq_t val = (seq_t)i;
        int len = digit_len(val);
        if (contains_digit(val, (unsigned)len))
        {
            count++;
            if (count == n)
                return val;
        }
        i++;
    }
}
