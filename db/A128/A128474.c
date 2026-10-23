/*
 * A128474  Largest number k such that 2^k divides n*(n-3)!.
 * https://oeis.org/A128474
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₂(n) + (n-3) - popcount₂(n-3)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v₂(x) is the exponent of 2 in x, popcount₂(x)
   counts the 1-bits in x's binary representation.  */
#include <stdint.h>
#include <stddef.h>

/* Return the largest power k such that 2^k divides n*(n-3)! for n >= 3. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                          /* undefined below offset, return 0 safely */

    idx_t nminus3 = n - 3;                  /* n-3 fits in idx_t (uint64_t) */

    /* count ones in binary representation of n-3 */
    unsigned int pop = __builtin_popcountll((uint64_t)nminus3);

    /* count trailing zeros in binary of n (i.e., v₂(n)) */
    unsigned int tz = __builtin_ctzll((uint64_t)n);

    /* compute a(n) using the derived formula */
    seq_t result = (seq_t)nminus3 + (seq_t)tz - (seq_t)pop;
    return result;
}
