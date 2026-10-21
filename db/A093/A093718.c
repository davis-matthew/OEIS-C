/*
 * A093718  a(n) = (n mod 3)^(n mod 2).
 * https://oeis.org/A093718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 3)^(n mod 2) with 0^0 treated as 1
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t used in calculations */

seq_t a(idx_t n)
{
    /* Compute n % 3 and n % 2 as uint64_t for compatibility */
    uint64_t mod3 = n % 3;
    uint64_t mod2 = n % 2;

    /* If exponent is 0 (n even) the result is always 1.
       If exponent is 1 (n odd) the result is n % 3, which is 0,1, or 2. */
    if (mod2 == 0)
        return (seq_t)1;
    else
        return (seq_t)mod3;
}
