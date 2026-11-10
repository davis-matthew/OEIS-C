/*
 * A094521  a(n) = gcd(n, A094457(n)).
 * https://oeis.org/A094521
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(n, A094457(n)), where A094457(n) is the largest k < n
 *            with L1 distance <= 2 between prime exponent vectors of n and k
 * Range:     u8 n <= 409, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 8608 (2026-11-07)
 * Helpers:   factor_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

static uint64_t A094457_prev_prime(uint64_t p)
{
    if (p <= 2)
        return 0;
    for (uint64_t q = p - 1; q >= 2; --q) {
        if (is_prime(q))
            return q;
        if (q == 2)
            break;
    }
    return 0;
}

static seq_t A094457_a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = (seq_t)1;
    seq_t N    = (seq_t)n;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];

        seq_t cand = N / (seq_t)p;
        if (cand > best)
            best = cand;

        if (exps[i] >= 2) {
            cand = N / (seq_t)p / (seq_t)p;
            if (cand > best)
                best = cand;
        }

        uint64_t q = A094457_prev_prime(p);
        if (q > 1) {
            cand = (N / (seq_t)p) * (seq_t)q;
            if (cand > best)
                best = cand;
        }
    }

    return best;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    return gcd((seq_t)n, A094457_a(n));
}
