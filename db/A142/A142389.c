/*
 * A142389  Primes congruent to 38 mod 47.
 * https://oeis.org/A142389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 38 (mod 47)
 * Range:     u8 n <= 1, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 877 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

static seq_t *prime_cache = NULL;   /* array of found primes   */
static idx_t  prime_cache_len = 0;  /* number of primes cached */
static idx_t  prime_cache_cap = 0;  /* current allocated capacity */

static idx_t next_k = 3;            /* counter for 47*k+38, start with k=3 (179) */

/* Allocate a larger array and copy existing primes into it. */
static void grow_prime_cache(idx_t needed)
{
    if (prime_cache_cap < needed) {
        idx_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 128;
        while (new_cap < needed) new_cap <<= 1;
        /* alloc_seq_array constructs seq_t objects appropriately. */
        seq_t *tmp = alloc_seq_array(new_cap);
        if (!tmp) exit(1);            /* allocation failed */
        for (idx_t i = 0; i < prime_cache_len; ++i)
            tmp[i] = prime_cache[i];  /* copy assignment */
        free_seq(prime_cache);        /* destroy old array */
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }
}

/* Ensure we have at least n primes in the cache. */
static void ensure_primes(idx_t n)
{
    while (prime_cache_len < n) {
        seq_t cand = ((seq_t)47 * (seq_t)next_k) + (seq_t)38;
        if (is_prime_big(cand)) {
            grow_prime_cache(prime_cache_len + 1);
            prime_cache[prime_cache_len++] = cand;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n<1; safe fallback */
    ensure_primes(n);
    return prime_cache[n - 1];
}
