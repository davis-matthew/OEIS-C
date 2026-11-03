/*
 * A099007  Primes of the form 6n^2 - 2n - 1.
 * https://oeis.org/A099007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 996 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: produce the n‑th prime of the form 6k²‑2k‑1 by
 * incrementally testing successive values of k and caching all found
 * primes. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Global static cache for memoisation across calls. */
static seq_t *cache = NULL;     /* array of primes found so far   */
static size_t cache_cap = 0;    /* allocated capacity of 'cache'  */
static size_t cache_len = 0;    /* number of primes stored        */
static idx_t next_k = 1;        /* next k to test in 6k²‑2k‑1     */

seq_t a(idx_t n)
{
    /* Keep generating candidates until we have at least n primes. */
    while (cache_len < n) {
        /* Compute candidate value 6k² – 2k – 1 using seq_t arithmetic. */
        seq_t v = (seq_t)6 * (seq_t)next_k * (seq_t)next_k
                  - (seq_t)2 * (seq_t)next_k
                  - (seq_t)1;

        /* Test primality. is_prime_big works for all seq_t sizes. */
        if (is_prime_big(v)) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_new(seq_t, new_cap);
                if (!tmp) abort();               /* OOM – abort */
                mem_copy(tmp, cache, cache_len * sizeof(seq_t));
                mem_free(cache);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = v;    /* store prime */
        }
        ++next_k;                     /* next k to examine */
    }

    return cache[n - 1];              /* 1‑based indexing */
}
