/*
 * A185358  The period of the sequence i^i (mod n) starts from i=a(n).
 * https://oeis.org/A185358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{p^e‖n} ( e - ((e-1) % p) ) (equivalent to a(n) =
 *            max_{p^e‖n} (1 - p*(1+floor(-e/p))) )
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* not expected, but safe */
        return 0;
    if (n == 1)
        return 1;

    /* Factor n using the provided utility. */
    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    uint64_t best = 1;          /* lower bound for a(n) */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute e - ((e-1) % p). For odd p this yields 1. */
        uint64_t mod = (p == 0) ? 0 : (e - 1) % p;
        uint64_t val = e - mod;

        if (val > best)
            best = val;
    }

    return (seq_t)best;
}
