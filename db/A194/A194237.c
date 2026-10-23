/*
 * A194237  a(n) = [Sum_{k=1..n} (3k/8)], where [ ]=floor, ( )=fractional
 *          part.
 * https://oeis.org/A194237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( Σ_{k=1..n} frac(3k/8) ) Here frac(x) = x -
 *            floor(x). The sequence starts with a(1) = 0
 * Range:     u8 n <= 73, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t _pref[8] = {0, 3, 9, 10, 14, 21, 23, 28};

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for offset 1 */

    idx_t q = n / 8;                     /* complete 8‑term cycles   */
    idx_t r = n % 8;                     /* remaining terms in last cycle */

    /* S(n) = Σ_{k=1..n} (3k mod 8) = 28*q + pref[r]   */
    seq_t sum = (seq_t)q * 28 + _pref[(size_t)r];

    /* a(n) = floor( S(n) / 8 ) = integer division. */
    return sum / 8;
}
