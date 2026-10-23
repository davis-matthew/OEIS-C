/*
 * A125124  Decimal expansion of the flattening (inverse) of the World
 *          Geodetic System 1984 ellipsoid, second upgrade.
 * https://oeis.org/A125124
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/f for the WGS 84 ellipsoid inverse flattening
   (1/f = 298.257223563).  a(n) returns the n‑th term, where n ≥ 3.
   The digits of the number (without the decimal point) form the sequence:
   2,9,8,2,5,7,2,2,3,5,6,3.  For n > 14 the sequence is defined to be 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Padding for n < 3 or beyond the known digits */
    if (n < 3)
        return (seq_t)0;
    if (n > 14)
        return (seq_t)0;   /* digits after the finite decimal expansion are 0 */

    /* The number 298.257223563 as an integer without the decimal point
       (i.e., 298257223563).  The sequence is the decimal digits of this integer. */
    const uint64_t number = 298257223563ULL;

    /* Pre‑computed powers of 10 from 10⁰ to 10¹¹. */
    static const uint64_t pow10[12] = {
        1ULL,
        10ULL,
        100ULL,
        1000ULL,
        10000ULL,
        100000ULL,
        1000000ULL,
        10000000ULL,
        100000000ULL,
        1000000000ULL,
        10000000000ULL,
        100000000000ULL
    };

    /* Convert the 1‑based index n to a 0‑based position inside the 12‑digit number. */
    /* For n = 3 (first term) we want the most significant digit at position 0. */
    unsigned idx = (unsigned)(n - 3);            /* 0 .. 11 */
    unsigned shift = 11 - idx;                   /* 11 .. 0 */

    uint64_t digit = (number / pow10[shift]) % 10ULL;
    return (seq_t)digit;
}
