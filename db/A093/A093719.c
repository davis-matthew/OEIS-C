/*
 * A093719  a(n) = (n mod 2)^(n mod 3).
 * https://oeis.org/A093719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 2)^(n mod 3)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 9481 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: 0^0 is defined as 1 here, so the value is 1 whenever n is odd or n is divisible by 3. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic with period 6: 1,1,0,1,0,1 */
    if ((n & 1ULL) || ((n % 3ULL) == 0ULL))
        return (seq_t)1;
    return (seq_t)0;
}
