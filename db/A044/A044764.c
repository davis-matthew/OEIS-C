/*
 * A044764  Numbers n such that string 5,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning integers for decimal representation containing
 * the substring "51" but where the next integer n+1 does not contain that
 * substring. */
 
#include <stdbool.h>
#include <stdint.h>
 
static bool has51(seq_t x)
{
    if (x == 0)
        return false;
    unsigned char digits[128];
    int pos = 0;
    while (x > 0)
    {
        digits[pos++] = (unsigned char)(x % 10);
        x /= 10;
    }
    for (int i = pos - 1; i > 0; --i)
        if (digits[i] == 5 && digits[i - 1] == 1)
            return true;
    return false;
}
 
seq_t a(idx_t n)
{
    seq_t cur = 1;
    idx_t found = 0;
    while (found < n)
    {
        if (has51(cur))
        {
            seq_t next = cur + 1;
            if (!has51(next))
            {
                ++found;
                if (found == n)
                    return cur;
            }
        }
        ++cur;
    }
    return cur; /* unreachable */
}
