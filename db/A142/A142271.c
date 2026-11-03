/*
 * A142271  Primes congruent to 22 mod 43.
 * https://oeis.org/A142271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 154, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 926 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A142271: Primes congruent to 22 (mod 43)
 * The sequence is defined as the increasing list of primes p such that
 * p ≡ 22 (mod 43).  The nth element a(n) can be found by generating
 * successive primes that satisfy this congruence.  The implementation
 * generates terms on demand, caching them so repeated calls are fast.
 * It supports both fixed‑size unsigned integer types and arbitrary‑precision
 * seq_t (via the helper is_prime_big).  The search uses only integer
 * arithmetic and the primality test; no floating point is used.
 *
 * The first few terms (for reference) are:
 * 151, 409, 839, 1097, 1613, 1699, 1871, 2129, 2473, …
 */

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

/* static cache for computed terms */
static seq_t *cache = NULL;   /* array of computed primes */
static idx_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_len = 0;   /* number of elements stored */

/*
 * Expand the cache to at least 'need' elements, generating new terms
 * as required.  The function calls is_prime_big to test primality.
 */
static void ensure_cache(idx_t need)
{
    if (cache == NULL) {
        /* initial allocation */
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) {
            /* allocation failure – abort program */
            abort();
        }
        /* first term is 151 */
        cache[0] = (seq_t)151;
        cache_len = 1;
    }

    while (cache_len < need) {
        /* generate next candidate congruent to 22 (mod 43) */
        seq_t cand = cache[cache_len - 1] + (seq_t)43;
        /* find the next prime in this residue class */
        while (!is_prime_big(cand))
            cand += (seq_t)43;
        /* enlarge cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;  /* double size */
            cache = mem_resize(seq_t, cache, cache_cap);
            if (cache == NULL) abort();
        }
        cache[cache_len++] = cand;
    }
}

/* MAIN FUNCTION TO EXPORT */
seq_t a(idx_t n)
{
    if (n < 1) 
        return (seq_t)0;   /* undefined for n = 0, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
