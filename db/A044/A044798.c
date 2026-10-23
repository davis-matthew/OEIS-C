/*
 * A044798  Numbers n such that string 8,5 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer m > 0 such that decimal representation *
 *            of m contains the substring "85" but m+1 does not
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if n's decimal string contains "85". */
static bool contains85(seq_t n)
{
    seq_t x = n;
    /* Each iteration checks one pair of adjacent decimal digits. */
    while (x >= 10) {
        if (x % 100 == 85)
            return true;
        x /= 10;
    }
    return false;
}

/* Return the nth term of A044798 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0, but safe return */

    idx_t found = 0;
    seq_t current = 1;

    for (;;) {
        if (contains85(current) && !contains85(current + 1)) {
            ++found;
            if (found == n)
                return current;
        }
        ++current;
    }
}
