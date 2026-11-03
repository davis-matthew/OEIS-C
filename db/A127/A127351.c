/*
 * A127351  Prime numbers n such that A127350(k) = 2*n for some k.
 * https://oeis.org/A127351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127351
   Prime numbers n such that
   A127350(k) = Σ_{i=k}^{k+3} Σ_{j=i+1}^{k+4} prime(i) * prime(j) = 2*n
   for some integer k. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* --------- static state ------------------------------------------- */

static seq_t *seq_cache = NULL;          /* cached sequence values           */
static size_t seq_cache_len = 0;         /* number of stored terms           */
static size_t seq_cache_cap = 0;         /* allocated space for cache        */

static uint64_t *prime_list = NULL;      /* list of primes up to limit      */
static size_t prime_count = 0;           /* number of primes stored         */
static size_t prime_limit = 0;           /* current sieve limit             */

static idx_t processed_k = 0;            /* next k to process                */

/* --------- helpers ----------------------------------------------- */

/* Append a value to the cache, growing it if necessary */
static void seq_append(seq_t val)
{
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        if (!seq_cache) {
            seq_cache = seq_alloc(new_cap);
            seq_cache_cap = new_cap;
        } else {
            seq_t *tmp = seq_realloc(seq_cache, seq_cache_cap, new_cap);
            if (!tmp) return;              /* allocation failure – ignore */
            seq_cache = tmp;
            seq_cache_cap = new_cap;
        }
    }
    seq_cache[seq_cache_len++] = val;
}

/* Simple sieve that stores all primes ≤ limit into prime_list */
static void build_sieve(size_t limit)
{
    unsigned char *isprime = (unsigned char *)malloc(limit + 1);
    if (!isprime) return;

    memset(isprime, 1, limit + 1);
    isprime[0] = isprime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                isprime[j] = 0;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (isprime[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(isprime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (isprime[i]) new_primes[idx++] = i;

    free(isprime);
    free(prime_list);
    prime_list = new_primes;
    prime_count = cnt;
    prime_limit = limit;
}

/* Ensure we have at least 'min_needed' primes; grow sieve if necessary */
static void ensure_primes(size_t min_needed)
{
    if (!prime_list) {
        build_sieve(2000000u);                /* initial limit ~148k primes  */
        if (prime_count < min_needed)
            build_sieve(prime_limit * 2 + 1);
    } else if (prime_count < min_needed) {
        build_sieve(prime_limit * 2 + 1);
    }
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    /* initialise prime tables once */
    ensure_primes(5);

    /* extend the cache until it contains at least n terms */
    while (seq_cache_len < (size_t)n) {
        /* make sure we can access primes[processed_k + 4] */
        if ((size_t)processed_k + 5 > prime_count) {
            ensure_primes((size_t)processed_k + 5);
            continue;                          /* recheck after growing */
        }

        /* compute sum of all pairwise products of the five primes */
        uint64_t sum = 0;
        for (size_t i = 0; i < 5; ++i) {
            for (size_t j = i + 1; j < 5; ++j) {
                uint64_t p1 = prime_list[(size_t)processed_k + i];
                uint64_t p2 = prime_list[(size_t)processed_k + j];
                sum += p1 * p2;
            }
        }

        if ((sum & 1ULL) == 0) {
            seq_t candidate = (seq_t)(sum >> 1);   /* (sum/2) */
            if (is_prime_big(candidate))
                seq_append(candidate);
        }

        ++processed_k;
    }

    return seq_cache[n - 1];
}
