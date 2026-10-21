/*
 * A239442  a(n) = phi(n^7).
 * https://oeis.org/A239442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^6*phi(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 24, u64 n <= 568,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Compute phi(n) for a 64‑bit unsigned integer n */
static seq_t compute_phi_uint(uint64_t n)
{
    if (n <= 1) return (seq_t)n;

    seq_t result = (seq_t)n;
    /* Number of distinct primes in a 64‑bit integer is at most 16 */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        result -= result / (seq_t)primes[i];
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t phi_n = compute_phi_uint(n);
    seq_t n6    = pow_uint((uint64_t)n, 6);
    return n6 * phi_n;
}
