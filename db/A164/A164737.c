/*
 * A164737  a(n) = 8*a(n-2) for n > 2; a(1) = 5, a(2) = 12.
 * https://oeis.org/A164737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) for n>2, a(1)=5, a(2)=12
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)12;

    /* e = floor((n-1)/2) for odd n, (n-2)/2 for even n,
       i.e. the exponent of 8 in the recurrence. */
    idx_t e = (n - ((n & 1U) ? 1ULL : 2ULL)) >> 1;

    seq_t result = (n & 1U) ? (seq_t)5 : (seq_t)12;

    /* Multiply by 8^e = 2^(3e): shift left by 3*e bits. */
    result <<= (size_t)(e * 3ULL);

    return result;
}
