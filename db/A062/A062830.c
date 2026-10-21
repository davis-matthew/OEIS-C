/*
 * A062830  a(n) = #{ 0 <= k <= n : K(n, k) = 0 } where K(n, k) is the
 *          Kronecker symbol. This is the number of integers 0 <= k <= n that
 *          are not coprime to n.
 * https://oeis.org/A062830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - phi(n) + 1 for n ≥ 2, with a(0)=1, a(1)=0
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1u;
    if (n == 1u) return (seq_t)0u;
    seq_t nn = (seq_t)n;
    seq_t phi_n = phi(n);
    return nn - phi_n + (seq_t)1u;
}
