/*
 * A043753  Number of runs in the base 2 representation of n is congruent to 7
 *          mod 8.
 * https://oeis.org/A043753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A043753: a(n) = the n-th positive integer n for which the
   number of runs (consecutive groups of equal bits) in its binary
   representation is congruent to 7 modulo 8.
   The number of runs of n is popcount( n ^ (n >> 1) ).
   Example: n = 85 (1010101₂) has 7 runs; 85 % 8 == 5, but runs % 8 == 7.
*/

#include <stdint.h>
#include <stdbool.h>

/* external link: seq_t and idx_t are defined in the build environment */
seq_t a(idx_t n)
{
    /* Handle trivial case: the sequence is defined for n >= 1. */
    if (n == 0)
        return (seq_t)0;          /* undefined, just return 0 */

    idx_t m = 1;            /* candidate integer to test */
    idx_t found = 0;        /* number of qualifying integers found so far */

    while (found < n)
    {
        /* Compute number of runs of bits in m:
           runs = popcount( m ^ (m >> 1) ) */
        uint64_t x = (uint64_t)m ^ ((uint64_t)m >> 1);
#if defined(__GNUC__) || defined(__clang__)
        unsigned int runs = __builtin_popcountll(x);
#else
        /* Fallback: count bits manually */
        unsigned int runs = 0;
        while (x)
        {
            runs += (uint32_t)(x & 1u);
            x >>= 1;
        }
#endif
        if ((runs & 7u) == 7u)   /* runs % 8 == 7 */
            ++found;

        ++m;
    }

    /* m has been incremented after the last qualifying number was found */
    return (seq_t)(m - 1);
}
