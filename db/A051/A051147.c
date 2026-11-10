/*
 * A051147  Smallest m such that A051145(m) = 2^n.
 * https://oeis.org/A051147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search: iterate A051145 via a(m) = ((a(m-1) OR a(m-2)) + 1) AND
 *            NOT a(m-1), a(0)=0, a(1)=1, and return the first m with
 *            A051145(m) = 2^n (the iteration uses native 128-bit integers;
 *            the index m fits in 64 bits)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 target = ((unsigned __int128)1) << (unsigned)n;
    unsigned __int128 prev2 = 0;   /* A051145(m-1) */
    unsigned __int128 prev1 = 1;   /* A051145(m)   */
    uint64_t m = 1;

    while (prev1 != target)
    {
        unsigned __int128 cur = ((prev1 | prev2) + 1) & ~prev1;
        prev2 = prev1;
        prev1 = cur;
        ++m;
    }
    return (seq_t)m;
}
