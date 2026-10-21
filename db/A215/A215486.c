/*
 * A215486  n - 1 mod phi(n), where phi(n) is Euler's totient function.
 * https://oeis.org/A215486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) mod φ(n)
 * Range:     u8 n <= 344, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard, though n ≥ 1 in usage */
    seq_t N      = (seq_t)n;
    seq_t phiN   = calc_phi(N);
    if (phiN == 0) return 0;      /* another safety guard, φ(0) is undefined */
    return (N - 1) % phiN;
}
