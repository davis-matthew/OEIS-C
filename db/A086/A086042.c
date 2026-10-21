/*
 * A086042  Nontrivial numbers which are prime and yield another prime when
 *          their digits are sorted in ascending order.
 * https://oeis.org/A086042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = prime p such that p is not already in ascending digit
 *            order, and the prime obtained by sorting p’s decimal digits in
 *            ascending order (leading zeros omitted) is also prime. Offset 1.
 *            The function is computed by checking integers in increasing
 *            order, testing primality with a deterministic Mi...
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* --------------------------------- helpers --------------------------------- */

/* 64‑bit modular multiplication, avoiding overflow with 128‑bit intermediate */

/* 64‑bit modular exponentiation */
static inline uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e) {
        if (e & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin test for 64‑bit integers
   (works for all 64‑bit unsigned numbers). */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < (int)(sizeof(small_primes)/sizeof(small_primes[0])); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* write n‑1 as d·2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }
    /* deterministic bases for 64‑bit */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (int i = 0; i < (int)(sizeof(bases)/sizeof(bases[0])); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------ a ------------------------------------- */

/* a(idx_t) – OEIS A086042 */
seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term listed */
    if (n == (idx_t)0) return (seq_t)0;  /* undefined, but harmless */

    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;
    static idx_t cache_cap = 0;
    static uint64_t candidate = 2;   /* current integer to test for primality */

    /* grow cache if necessary */
    if (n > cache_cap) {
        cache_cap = n;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    while (cache_cnt < n) {
        /* find the next prime */
        while (true) {
            if (isPrime64(candidate)) break;
            if (candidate == 2) candidate = 3;
            else candidate += 2;   /* skip even numbers after 2 */
        }

        /* candidate is prime */
        uint64_t temp = candidate;
        uint8_t cnt[10] = {0};
        while (temp > 0) {
            cnt[temp % 10]++;
            temp /= 10;
        }

        /* build sorted ascending digits (leading zeros omitted) */
        uint64_t sorted = 0;
        for (int d = 0; d <= 9; ++d) {
            while (cnt[d]) {
                sorted = sorted * 10 + (uint64_t)d;
                --cnt[d];
            }
        }

        /* if sorted is a different prime, store the term */
        if (sorted != candidate && isPrime64(sorted)) {
            cache[cache_cnt++] = (seq_t)candidate;
        }

        /* move to next candidate */
        if (candidate == 2) candidate = 3;
        else candidate += 2;
    }

    return cache[n - 1];
}
