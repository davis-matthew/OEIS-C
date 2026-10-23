/*
 * A151682  a(n) = Sum_{k >= 0, k odd} binomial(wt(n+k),k+1), where wt() =
 *          A000120().
 * https://oeis.org/A151682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k odd ≥ 1} binomial( wt(n + k), k )
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* wt(x) is the binary weight A000120(x).  The sum is taken over
   all odd k, and binomial(n, k) is 0 when k > n.  */

/* Required building blocks are provided in ../utils/binomial.h. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k < 128; k += 2) {
        /* Compute n + k in 128 bits to avoid overflow on the upper 64
           bits of the natural number.  */
        unsigned __int128 nk = (unsigned __int128)n + k;

        /* Binary weight of the 128‑bit sum: popcount of low and high
           64-bit halves.  */
        int weight = __builtin_popcountll((uint64_t)nk) +
                     __builtin_popcountll((uint64_t)(nk >> 64));

        /* Add binomial(weight, k) only if weight >= k (otherwise it
           would be zero).  */
        if ((idx_t)k <= (idx_t)weight) {
            sum += binomial((idx_t)weight, k);
        }
    }
    return sum;
}
