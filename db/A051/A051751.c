/*
 * A051751  Cubes arising in A051750.
 * https://oeis.org/A051751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = p^3 where p = A051750(n), the n-th prime whose cube has
 *            no digit 0 (search over primes)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static bool A051751_zeroless(seq_t t)
{
    if (t == (seq_t)0) return false;
    while (t > (seq_t)0) {
        if (t % (seq_t)10 == (seq_t)0) return false;
        t /= (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t cand = 2;

    if (n == 0) return (seq_t)0;
    while (len < (size_t)n) {
        if (is_prime((idx_t)cand)) {
            seq_t c = (seq_t)cand * (seq_t)cand * (seq_t)cand;
            if (A051751_zeroless(c)) {
                if (len == cap) {
                    cap = cap ? cap * 2 : 64;
                    cache = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
                    if (!cache) abort();
                }
                cache[len++] = cand;
            }
        }
        cand++;
    }
    uint64_t p = cache[(size_t)n - 1];
    return (seq_t)p * (seq_t)p * (seq_t)p;
}
