/*
 * A124379  Numbers n such that 120n-1, 120n+1 are twin primes.
 * https://oeis.org/A124379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 291, u32 n <= 5746, u64 n <= 5746,
 *            u128 n <= 5746, big any n
 * Validated: standard, pass, 72 terms through n = 5625 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124379: numbers n such that 120n-1 and 120n+1 are twin primes. */
/* The sequence is defined for n ≥ 1; a(1)=2, a(2)=5, ... as listed on OEIS. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Static cache for memoised terms (1‑indexed). */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static idx_t  cache_sz  = 0;

/* Next n to examine for the property. */
static idx_t next_k = 1;

/* Ensure the cache can hold at least |need| elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        return; /* allocation failure – future calls will silently fail */
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether 120*k-1 and 120*k+1 are both prime. */
static bool candidate(idx_t k)
{
    seq_t mult = (seq_t)120 * (seq_t)k;
    seq_t a = mult - (seq_t)1;
    seq_t b = mult + (seq_t)1;
    return is_prime_big(a) && is_prime_big(b);
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but kept for type consistency */

    /* Generate terms until we have at least n entries. */
    while (cache_sz < n) {
        if (candidate(next_k)) {
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
