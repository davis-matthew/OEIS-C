/*
 * A215486  n - 1 mod phi(n), where phi(n) is Euler's totient function.
 * https://oeis.org/A215486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) mod φ(n)
 * Range:     u8 n <= 344, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 8675 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard, though n ≥ 1 in usage */
    seq_t N      = (seq_t)n;
    seq_t phiN   = totient_seq(N);
    if (phiN == 0) return 0;      /* another safety guard, φ(0) is undefined */
    return (N - 1) % phiN;
}
