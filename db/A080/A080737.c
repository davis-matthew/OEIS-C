/*
 * A080737  a(1) = a(2) = 0; for n > 2, the least dimension of a lattice
 *          possessing a symmetry of order n.
 * https://oeis.org/A080737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ a(pᵉ) over prime powers of n, where a(2ᵉ) = 2^(e-1) for
 *            e>1, a(pᵉ) = φ(pᵉ) = p^(e-1)*(p-1) for odd primes p, a(1) = a(2)
 *            = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t comp = 0;

        if (p == 2)
        {
            if (e > 1)
                comp = ((seq_t)1ULL << (e - 1));
        }
        else
        {
            comp = pow_uint(p, (idx_t)(e - 1)) * (seq_t)(p - 1);
        }

        res += comp;
    }

    return res;
}
