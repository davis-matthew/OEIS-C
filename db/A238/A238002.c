/*
 * A238002  Count with multiplicity of prime factors of n in (n - 1)!.
 * https://oeis.org/A238002
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{p|n} v_p((n‑1)!) where * v_p(k!) = ⌊k/p⌋ + ⌊k/p²⌋ +
 *            ⌊k/p³⌋ + …
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    uint64_t m = (uint64_t)(n - 1);      /* compute (n‑1)! */

    /* factor n to get its distinct prime factors */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 64;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* sum the exponents of these primes in (n‑1)! */
    uint64_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t v = 0;
        uint64_t t = m;
        while (t) {
            t /= p;
            v += t;
        }
        total += v;
    }

    return (seq_t)total;
}
