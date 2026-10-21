/*
 * A094457  a(n) is the largest number k < n in a neighborhood of n defined by
 *          prime factorization, specifically, A127185(n,k) <= 2.
 * https://oeis.org/A094457
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest k < n with A127185(n,k) ≤ 2 (L1 distance between
 *            prime exponent vectors ≤ 2)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* Find the largest prime < p.  Returns 0 if none (p ≤ 2). */
static uint64_t prev_prime(uint64_t p)
{
    if (p <= 2)
        return 0;
    for (uint64_t q = p - 1; q >= 2; --q) {
        if (is_prime(q))
            return q;
        if (q == 2)
            break;               /* next is 1, exit */
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* out of defined range; still safe */

    /* Factor n.  The arrays are large enough for all n in practice. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = (seq_t)1;        /* at least 1 */
    seq_t N    = (seq_t)n;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];

        /* Distance 1: divide by p */
        seq_t cand = N / (seq_t)p;
        if (cand > best)
            best = cand;

        /* Distance 2: divide by p² (if exponent ≥ 2) */
        if (exps[i] >= 2) {
            cand = N / (seq_t)p / (seq_t)p;
            if (cand > best)
                best = cand;
        }

        /* Distance 2: replace one p by the largest smaller prime */
        uint64_t q = prev_prime(p);
        if (q > 1) {
            cand = (N / (seq_t)p) * (seq_t)q;
            if (cand > best)
                best = cand;
        }
    }

    return best;
}
