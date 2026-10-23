/*
 * A040696  Continued fraction for sqrt(723).
 * https://oeis.org/A040696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative definition of A040696.
   a(0) = 26
   For n >= 1:
       let e = valuation_2(n)  (exponent of 2 in n)
       a(n) = 1   if e == 0
               7   if e == 1
               52  if e >= 2
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)26;

    /* Count the exponent of 2 in n. */
    uint64_t m = (uint64_t)n;
    uint64_t e = 0;
    while ((m & 1ULL) == 0ULL) {
        e++;
        m >>= 1;
    }

    if (e == 0)
        return (seq_t)1;
    else if (e == 1)
        return (seq_t)7;
    else
        return (seq_t)52;
}
