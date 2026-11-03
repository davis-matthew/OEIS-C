/*
 * A097379  Numbers m such that 1+SquareFreeKernel(m) is prime.
 * https://oeis.org/A097379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer m such that 1 + squarefree_kernel(m) is
 *            prime
 * Range:     u8 n <= 77, u16 n <= 7876, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 7399 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches sequentially, caching results. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
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

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_len = 0;         /* number of values currently cached */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static uint64_t next_candidate = 1;  /* next integer to be tested */

/* Grow the cache to accommodate at least `needed` entries. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
    if (!new_buf) {
        /* Allocation failed – abort; in typical environments this is fatal. */
        abort();
    }
    cache = new_buf;
    cache_cap = new_cap;
}

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) /* Not part of OEIS; return 0 for safety. */
        return 0;

    /* Extend cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate;

        /* Compute the square‑free kernel of m by multiplying the
           distinct prime factors returned by factor_u64. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt;
        factor_u64(m, primes, exps, &pcnt);

        seq_t kernel = 1;
        for (size_t i = 0; i < pcnt; ++i)
            kernel = kernel * (seq_t)primes[i];

        /* If kernel+1 is prime, store m in the cache. */
        if (is_prime_big(kernel + (seq_t)1)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
