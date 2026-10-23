/*
 * A121384  a(n) = ceiling(n*e).
 * https://oeis.org/A121384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*e)
 * Range:     u8 n <= 93, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* e ≈ 2.71828182845904523536 …  */
    /* Use 18 decimal digits of precision:     */
    /*  e ≈ NUM / SCALE   with                */
    /*  NUM   = 2718281828459045235            */
    /*  SCALE = 10^18 = 1000000000000000000       */
    const unsigned __int128 NUM   = 2718281828459045235ULL;
    const unsigned __int128 SCALE = 1000000000000000000ULL;

    /* Compute ceil(n * NUM / SCALE) exactly */
    unsigned __int128 prod = (unsigned __int128)n * NUM + (SCALE - 1);
    return (seq_t)(prod / SCALE);
}
