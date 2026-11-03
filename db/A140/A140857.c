/*
 * A140857  Primes of the form 210k + 73.
 * https://oeis.org/A140857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 210 k + 73 (OEIS A140857) */

#include <stdlib.h>
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

/* cache for already found primes */
static seq_t *cache      = NULL;   /* array of primes */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of primes stored */
static idx_t next_k       = 0;     /* next k to check */

/* enlarge cache to hold at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap < 8 ? 8 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache     = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 0 */
        return 0;

    while (cache_len < n) {
        seq_t cand = (seq_t)73 + (seq_t)210 * (seq_t)next_k;
        if (is_prime_big(cand))
            ensure_cache_capacity(cache_len + 1), cache[cache_len++] = cand;
        next_k++;
    }
    return cache[n - 1];
}
