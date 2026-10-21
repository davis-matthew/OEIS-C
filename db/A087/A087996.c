/*
 * A087996  Residues when (n+rev[n]) is divided by abs(n-rev[n]); zero if
 *          n=rev[n] (when n is palindromic), or when the quotient is an
 *          integer (see A087993).
 * https://oeis.org/A087996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A087996: Residues of (n+rev[n]) mod |n-rev[n] |
   a(n) = 0 if n is a palindrome or (n+rev[n]) is an integer multiple of |n-rev[n] */
#include <stdint.h>

static idx_t rev_decimal(idx_t n)
{
    idx_t rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n = n / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* Compute rev[n] */
    idx_t rev = rev_decimal(n);

    /* Compute absolute difference |n - rev| */
    idx_t diff;
    if (n >= rev)
        diff = n - rev;
    else
        diff = rev - n;

    if (diff == 0)
        return (seq_t)0;                    /* palindrome */

    /* Sum n + rev[n] */
    idx_t sum = n + rev;

    /* If (n+rev) is a multiple of |n-rev|, return 0 */
    if (sum % diff == 0)
        return (seq_t)0;

    /* Return residue modulo |n-rev| */
    return (seq_t)(sum % diff);
}
