/*
 * A210511  Primes formed by concatenating k, k, and 1 for k >= 1.
 * https://oeis.org/A210511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 959 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210511: primes formed by concatenating k, k, and 1 for k ≥ 1 */
/* a(n) is the n-th prime of the form concat(k, k, 1), with n ≥ 1 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;          /* cached primes */
static size_t cache_cap = 0;         /* capacity of 'cache' */
static idx_t  cache_count = 0;       /* number of primes stored */
static idx_t  next_k = 1;            /* next k to test */

/* Ensure the cache can hold at least 'needed' primes. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        /* Allocation failure: leave cache unchanged. */
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Build the prime candidate for a given k:
 *   value = concat(k, k, 1) = k * 10^(d+1) + k * 10 + 1
 * where d = number of decimal digits of k. */
static seq_t candidate_for_k(idx_t k)
{
    int d = num_digits_idx(k);                       /* digits of k */
    seq_t pow = ipow((seq_t)10, (seq_t)(d + 1));
    seq_t part1 = (seq_t)k * pow;            /* k * 10^(d+1) */
    seq_t part2 = (seq_t)k * 10;             /* k * 10      */
    return part1 + part2 + (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n = 0; sequence offset = 1 */

    /* Generate primes until we have at least n of them. */
    while (cache_count < n) {
        seq_t candidate = candidate_for_k(next_k);
        if (is_prime_big(candidate)) {
            ensure_cache_capacity(cache_count + 1);
            cache[cache_count++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];
}
