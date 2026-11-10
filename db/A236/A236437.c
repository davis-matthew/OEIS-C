/*
 * A236437  Primes which occur in their proper place in A236174.
 * https://oeis.org/A236437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th prime p such that max prime among base-k (k=2..10)
 *            readings of p, read in decimal, equals p; found by search
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 28, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9632 (2026-11-07)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_prime_seq.h"

static bool A236437_ok(uint64_t p)
{
    for (int k = 2; k <= 9; ++k) {
        uint8_t d[64];
        int len = 0;
        uint64_t t = p;
        do { d[len++] = (uint8_t)(t % (uint64_t)k); t /= (uint64_t)k; } while (t);
        seq_t val = 0;
        for (int i = len - 1; i >= 0; --i) val = val * 10 + (seq_t)d[i];
        if (val > (seq_t)p && is_prime_seq(val)) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t cand = 2;

    while (cnt < (size_t)n) {
        uint64_t p = cand;
        cand = (cand == 2) ? 3 : cand + 2;
        if (!is_prime_u64(p)) continue;
        if (!A236437_ok(p)) continue;
        if (cnt == cap) {
            cap = cap ? cap * 2 : 64;
            terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            if (!terms) abort();
        }
        terms[cnt++] = p;
    }
    return (seq_t)terms[(size_t)n - 1];
}
