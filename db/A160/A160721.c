/*
 * A160721  First differences of A160720.
 * https://oeis.org/A160721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=1; a(n)=4*(2^popcount(n-1)-1) for n>1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    idx_t m = n - (idx_t)1;
    unsigned int pc = __builtin_popcountll((unsigned long long)m);

    /* Compute 2^pc - 1 using an unsigned 64‑bit intermediate. */
    unsigned long long val = 1ULL;
    for (unsigned int i = 0; i < pc; ++i)
        val <<= 1;

    val = val - 1ULL;   /* 2^popcount(n-1) - 1 */
    val = val * 4ULL;   /* times 4 */

    return (seq_t)val;
}
