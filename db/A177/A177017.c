/*
 * A177017  Primes prime(n) such that prime(n) + prime(n+1) + 1 is also prime.
 * https://oeis.org/A177017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 929, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 930 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A177017: primes p such that p + next_prime(p) + 1 is also prime. 
 *  This function returns the nth term (n >= 1, 1-based indexing).
 *  It uses a static cache to store computed terms and grows it as
 *  necessary.  Each call works independently of the order of calls.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against 0 */
        return (seq_t)0;

    static seq_t *cache = NULL;      /* dynamic array of found primes */
    static size_t cache_len = 0;     /* number of cached terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t last_prime = 0;  /* last prime processed */

    /* Lazy initialisation of cache */
    if (cache == NULL) {
        cache_cap = 1024;           /* arbitrary initial size */
        cache = seq_alloc_array(cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t p1, p2;

        /* Find the next candidate prime */
        if (last_prime == 0)
            p1 = next_prime(2);
        else
            p1 = next_prime(last_prime + 1);

        /* Find the subsequent prime */
        p2 = next_prime(p1 + 1);

        /* Compute p1 + p2 + 1 and test primality */
        seq_t sum = (seq_t)p1 + (seq_t)p2 + (seq_t)1;
        if (is_prime_big(sum)) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p1);
        }

        /* Update state for next iteration */
        last_prime = p1;
    }

    return cache[n - 1];
}
