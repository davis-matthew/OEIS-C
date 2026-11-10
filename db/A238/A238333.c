/*
 * A238333  The nonprime numbers formed from the primes in A238332.
 * https://oeis.org/A238333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p mod 10^(d-1), where p is the n-th term of A238332 (d =
 *            number of digits of p); found by a search over primes
 * Range:     u8 n <= 29, u16 n <= 2700, u32 n <= 2700, u64 n <= 2700,
 *            u128 n <= 2700, big any n
 * Validated: standard, pass, 71 terms through n = 2327 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    static uint64_t *suf = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t cand = 10;

    while (len < (size_t)n) {
        uint64_t p = cand++;
        if (!is_prime_u64(p)) continue;
        uint64_t p10 = 1;
        while (p10 * 10 <= p) p10 *= 10;
        uint64_t s = p % p10;
        if (is_prime_u64(s)) continue;
        bool seen = false;
        for (size_t i = 0; i < len; ++i) {
            if (suf[i] == s) { seen = true; break; }
        }
        if (seen) continue;
        if (len == cap) {
            size_t nc = cap ? cap * 2 : 64;
            suf = (uint64_t *)realloc(suf, nc * sizeof(uint64_t));
            cap = nc;
        }
        suf[len++] = s;
    }
    return (seq_t)suf[(size_t)n - 1];
}
