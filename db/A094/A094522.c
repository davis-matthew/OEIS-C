/*
 * A094522  a(n) = gcd(n, A064802(n)).
 * https://oeis.org/A094522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(n, A064802(n)); A064802 computed by factoring n and
 *            replacing one prime p by the next prime, keeping the minimum
 *            product
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-07)
 * Helpers:   gcd, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/gcd.h"

static uint64_t A094522_next_prime_after(uint64_t p)
{
    uint64_t cand = (p > 2) ? p + 1 : 3;
    if (cand % 2 == 0) cand++;
    for (;;) {
        if (is_prime_seq((seq_t)cand)) return cand;
        cand += 2;
    }
}

static size_t A094522_distinct_prime_factors(uint64_t n, uint64_t *out)
{
    size_t count = 0;
    if (n % 2 == 0) {
        out[count++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            out[count++] = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) out[count++] = n;
    return count;
}

static seq_t A094522_A064802_a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    size_t pcnt = A094522_distinct_prime_factors(n64, primes);
    seq_t best = 0;
    bool first = true;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t q = A094522_next_prime_after(p);
        seq_t cand = (seq_t)(n64 / p) * (seq_t)q;
        if (first || cand < best) {
            best = cand;
            first = false;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    return gcd((seq_t)n, A094522_A064802_a(n));
}
