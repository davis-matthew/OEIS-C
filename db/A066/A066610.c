/*
 * A066610  a(n) = 6^n mod n^6.
 * https://oeis.org/A066610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n mod n^6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n^6 as an unsigned __int128 to avoid overflow       */
    /*  n is at least 1 (offset 1 of the sequence)                  */
    unsigned __int128 mod  = (unsigned __int128)n *
                             n * n * n * n * n;    /* n^5 multiplied once more for n^6 */
    unsigned __int128 base = 6;
    unsigned __int128 result = 1;
    idx_t exp = n;                  /* preserve original exponent */

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return (seq_t)result;           /* result < mod; fits in seq_t when appropriate */
}
