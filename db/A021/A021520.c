/*
 * A021520  Decimal expansion of 1/516.
 * https://oeis.org/A021520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit (starting at 0) in the decimal expansion of
 *            1/516
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is obtained by simulating long division: */
#include <stdint.h>

/* Sequence type definitions are assumed to be provided elsewhere. */
seq_t a(idx_t n)
{
    /* Quotient of 1/516.  The remainder after i steps is < 516
       so we can safely hold it in a normal integer type with no overflow. */
    const unsigned int DENOM = 516;
    unsigned int rem = 1;          /* initial remainder (1 % 516) */
    unsigned int digit = 0;        /* current digit */

    /* Compute digits up to the n-th.  The loop inclusive yields
       the digit at index n.  All arithmetic is done on 32-bit
       integers which are large enough for the intermediate
       values (< 10*516 < 6000). */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem = rem * 10;            /* remainder * 10 */
        digit = rem / DENOM;       /* next decimal digit */
        rem   = rem % DENOM;       /* new remainder */
    }

    /* The digit is always 0..9, safely cast back to seq_t. */
    return (seq_t)digit;
}
