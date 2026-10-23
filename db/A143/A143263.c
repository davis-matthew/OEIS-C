/*
 * A143263  A positive integer k is included if the binary representation of
 *          the number of divisors of k occurs in the binary representation of
 *          k at least once.
 * https://oeis.org/A143263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A143263: a(n) is the nth positive integer k such that
   the binary representation of τ(k) (the divisor count of k)
   appears as a contiguous substring of the binary representation of k.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* return the bit length of a positive integer x (x>0) */
static int bitlen(uint64_t x)
{
    int l = 0;
    while (x) { ++l; x >>= 1; }
    return l;
}

/* true if binary representation of d occurs in binary of k */
static bool binary_contains(uint64_t k, uint64_t d)
{
    int lenk = bitlen(k);
    int lend = bitlen(d);
    if (lend > lenk) return false;

    for (int pos = 0; pos + lend <= lenk; ++pos) {
        bool ok = true;
        for (int j = 0; j < lend; ++j) {
            uint64_t bitk = (k >> (lenk - 1 - pos - j)) & 1ULL;
            uint64_t bitd = (d >> (lend - 1 - j)) & 1ULL;
            if (bitk != bitd) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

/* compute divisor count τ(n) using factor_u64 */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) tau *= (exps[i] + 1ULL);
    return tau;
}

/* dynamic cache for the sequence */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;
static uint64_t next_k       = 1;   /* next positive integer to test */

/* grow cache if necessary */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1; behaviour for n==0 is irrelevant */
    while (cache_len < n) {
        uint64_t k = next_k++;
        uint64_t tau = divisor_count(k);
        if (binary_contains(k, tau)) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)k;   /* cast to generic seq_t */
        }
    }
    return cache[n - 1];
}
