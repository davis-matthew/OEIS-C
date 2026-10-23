/*
 * A173531  a(0)=0: For n>0, a(n) = A060632(n)*A060632(n-1).
 * https://oeis.org/A173531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^( popcount(n>>1) + popcount((n-1)>>1) ), a(0)=0
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 9026, u64 n <= 9026,
 *            u128 n <= 9026, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Split n into halves */
    idx_t h1 = n >> 1;          /* n/2 */
    idx_t h2 = (n - 1) >> 1;    /* (n-1)/2 */

    /* Count 1‑bits in both halves */
    unsigned int pop = __builtin_popcountll((unsigned long long)h1) +
                       __builtin_popcountll((unsigned long long)h2);

    /* 2^pop fits into seq_t (built‑in or arbitrary precision) */
    return pow2((idx_t)pop);
}
