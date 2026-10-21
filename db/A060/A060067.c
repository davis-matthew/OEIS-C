/*
 * A060067  Largest power of n which divides n!.
 * https://oeis.org/A060067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 23, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n^k, where k = min_{p|n} ⌊ v_p(n!) / v_p(n) ⌋
 * (largest exponent such that n^k divides n!)
 *  v_p(n!) is Legendre's formula: Σ_{i≥1} ⌊ n / p^i ⌋
 */

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    /* Base case: 1 divides 1! exactly once. */
    if (n <= 1U)
        return (seq_t)1;

    /* Factorise n.  For n ≤ 10000 this is trivial. */
    uint64_t pr[64], exp[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, exp, &cnt);

    /* Compute the minimal exponent allowed by each prime factor. */
    unsigned long long best = ULLONG_MAX;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = exp[i];

        /* Legendre's formula: exponent of p in n!. */
        uint64_t vp = 0;
        uint64_t m  = (uint64_t)n;
        while (m) {
            m /= p;
            vp += m;
        }

        unsigned long long cur = vp / e;      /* allowed exponent for this prime */
        if (cur < best)
            best = cur;
    }

    /* Compute n^best with binary exponentiation in seq_t. */
    seq_t base   = (seq_t)n;
    seq_t result = (seq_t)1;
    unsigned long long e = best;
    while (e) {
        if (e & 1ULL)
            result = result * base;
        base = base * base;
        e >>= 1ULL;
    }

    return result;
}
