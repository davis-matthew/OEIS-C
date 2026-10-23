/*
 * A164910  Partial sums of A088748.
 * https://oeis.org/A164910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + \sum_{k=0}^{n-1} popcount( k ^ (k >> 1) )
 * Range:     u8 n <= 63, u16 n <= 8738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where popcount(x) is the number of 1‑bits in the binary representation of x.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial term from the summation formula */
    seq_t sum = (seq_t)n;

    /* Accumulate popcount of Gray codes for k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k) {
        idx_t gray = k ^ (k >> 1);
        /* __builtin_popcountll is available on GCC/Clang and returns
           an unsigned int.  We cast to seq_t before adding. */
        sum += (seq_t)__builtin_popcountll((unsigned long long)gray);
    }
    return sum;
}
