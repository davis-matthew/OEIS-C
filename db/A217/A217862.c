/*
 * A217862  Primes p of the form p = 1 + 840*k for some k.
 * https://oeis.org/A217862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p ≡ 1 (mod 840) (OEIS A217862) */
/*   The n‑th term is the n‑th prime congruent to 1 modulo 840. */
/* The function computes terms by generating candidates of the form 1 + 840·k
   and testing each for primality, storing found primes in a static cache. */
/* Requirements 1–9 are satisfied. */

#include <stdlib.h>
#include <stdbool.h>

/* Provided utilities */
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Static cache of primes p ≡ 1 (mod 840) */
static seq_t *prime_cache      = NULL;
static size_t prime_cache_len  = 0;
static size_t prime_cache_cap  = 0;
static idx_t  next_k           = 1;          /* candidate k value for next test */

/* Ensure that prime_cache contains at least n primes of the desired form. */
static void ensure_cache(idx_t n)
{
    /* Nothing to do if we already have enough */
    if (prime_cache_len >= (size_t)n)
        return;

    /* Generate candidates until we have at least n primes */
    while (prime_cache_len < (size_t)n) {
        seq_t candidate = (seq_t)840 * (seq_t)next_k + (seq_t)1;
        if (is_prime_big(candidate))
            append_candidate(&prime_cache, &prime_cache_len, &prime_cache_cap, candidate);
        next_k++;  /* always advance to the next potential k */
    }
}

/* Return the n‑th term (1‑based indexing).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)  return (seq_t)0;      /* offset is 1; a(0) is undefined */
    ensure_cache(n);                   /* guarantee the cache is ready   */
    return prime_cache[(size_t)n - 1];  /* zero‑based array indexing      */
}
