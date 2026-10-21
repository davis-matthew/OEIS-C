/*
 * A060785  a(n) = 3*(n - 2)*(5*n -11).
 * https://oeis.org/A060785
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n - 2)*(5*n - 11)
 * Range:     u8 n <= 6, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }
    /* Compute using unsigned __int128 to avoid intermediate overflow */
    unsigned __int128 tn = (unsigned __int128)(seq_t)n;
    unsigned __int128 val = (unsigned __int128)3 * (tn - 2) * (5 * tn - 11);
    return (seq_t)val;
}
