/*
 * A196795  a(n) = Sum_{k=0..n} binomial(n,k)*3^k*(k+1)^(n-k).
 * https://oeis.org/A196795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*3^k*(k+1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow3 = 1;          /* 3^k for current k */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin     = binomial(n, k);          /* binomial(n,k)   */
        seq_t pow     = ipow_u64_idx(k + 1, n - k);   /* (k+1)^(n-k)     */
        seq_t term    = bin * pow3 * pow;         /* bin*3^k*(k+1)^(n-k) */
        result += term;
        pow3 *= 3;                                 /* advance 3^k         */
    }
    return result;
}
