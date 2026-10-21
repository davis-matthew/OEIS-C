/*
 * A093003  Number of composite numbers among next n numbers.
 * https://oeis.org/A093003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = comp(n(n+1)/2) – comp(n(n-1)/2) where comp(m) = number of
 *            composite numbers ≤ m = m – pi(m) – 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

static seq_t comp_leq(idx_t m)
{
    if (m < 2) return (seq_t)0;                 /* 0 and 1 are not composite */
    seq_t primes = compute_pi(m);               /* pi(m) – primes ≤ m */
    return (seq_t)m - primes - (seq_t)1;        /* composites ≤ m */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* undefined for n < 1, return 0 */
    idx_t high = n * (n + 1) / 2;               /* n(n+1)/2 */
    idx_t low  = n * (n - 1) / 2;               /* n(n-1)/2 */
    return comp_leq(high) - comp_leq(low);
}
