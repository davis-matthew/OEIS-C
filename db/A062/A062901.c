/*
 * A062901  Number and its reversal are both multiples of 7.
 * https://oeis.org/A062901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer k such that k % 7 == 0 and
 *            reverse(k) % 7 == 0
 * Range:     u8 n <= 3, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Reverse the decimal digits of a non‑negative integer.  */
static seq_t reverse_digits(seq_t v)
{
    seq_t rev = 0;
    while (v > 0)
    {
        rev = rev * 10 + (v % 10);
        v   = v / 10;
    }
    return rev;
}

/* External linkage: use seq_t and idx_t typedefs supplied by the build. */
seq_t a(idx_t n)
{
    /* Count how many qualifying numbers we have found. */
    idx_t  found = 0;
    seq_t  candidate = 7;          /* start with the first multiple of 7 */

    while (1)
    {
        /* candidate is always a multiple of 7 */
        if (reverse_digits(candidate) % 7 == 0)
        {
            ++found;
            if (found == n)
                return candidate;
        }
        candidate += 7;            /* next multiple of 7 */
    }
}
