/*
 * A227827  a(0)=3; for n>0, a(n) is the smallest (odd or even) prime p such
 *          that the n-th prime p_n divides a(n-1)+p.
 * https://oeis.org/A227827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=3; for n>0 the smallest prime p such that the n‑th prime p_n divides a(n‑1)+p. */

/*  The implementation uses a cached array of values and a cached array of the first primes.
    For each new n it grows the arrays if necessary and compute the terms in O(n) time.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility function prototypes needed for this implementation */
#include <string.h>
#include <math.h>
#include "../utils/sieve_first_n_primes.h"
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

/* Cache for computed sequence values */
static seq_t *seq_cache       = NULL;   /* array of a(i) */
static idx_t  seq_cache_max   = (idx_t)-1; /* maximum index computed, (idx_t)-1 means none yet */
static size_t seq_cache_len   = 0;      /* allocated length of seq_cache */

/* Cache for primes (indexed from 1) */
static uint64_t *prime_cache   = NULL;   /* array of first primes */
static size_t   prime_cache_len = 0;     /* number of primes stored */

/* Helper: ensure that prime_cache contains at least k primes (1‑based). */
static void ensure_primes(size_t k)
{
    if (prime_cache_len >= k)
        return;

    /* Allocate primes list up to k primes */
    uint32_t *tmp32 = NULL;
    size_t   cnt32 = 0;
    sieve_first_n_primes((idx_t)k, &tmp32, &cnt32);   /* cnt32 == k */

    /* Allocate/resize the cache */
    uint64_t *new_cache = (uint64_t *)malloc(cnt32 * sizeof(uint64_t));
    for (size_t i = 0; i < cnt32; ++i)
        new_cache[i] = (uint64_t)tmp32[i];

    /* Free old cache and old temp array */
    free(prime_cache);
    free(tmp32);

    prime_cache   = new_cache;
    prime_cache_len = cnt32;
}

/* Helper: ensure that seq_cache is large enough for indices up to n. */
static void ensure_seq_cache(idx_t n)
{
    if (seq_cache_len > (size_t)n) return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = mem_resize(seq_t, seq_cache, new_len);
    if (!new_cache) exit(EXIT_FAILURE);
    seq_cache = new_cache;
    seq_cache_len = new_len;
}

/* Helper: find the smallest prime p such that (prev + p) is divisible by pk. */
static seq_t find_smallest_prime(seq_t prev, uint64_t pk)
{
    /* Compute residue r = (-prev) mod pk */
    seq_t pk_seq = (seq_t)pk;
    seq_t r_seq  = (pk_seq - (prev % pk_seq)) % pk_seq;
    uint64_t r = (uint64_t)r_seq;

    /* candidate starts at r, but must be at least 2 */
    uint64_t cand = r;
    if (cand < 2) {
        uint64_t add = (2 - cand + pk - 1) / pk; /* ceil((2 - cand)/pk) */
        cand += add * pk;
    }

    /* Walk over numbers congruent to r modulo pk until a prime is found */
    while (true) {
        if (is_prime_big((seq_t)cand))
            return (seq_t)cand;
        cand += pk;
    }
}

/* The required function */
seq_t a(idx_t n)
{
    /* Initialize cache for a(0) */
    if (seq_cache_max == (idx_t)-1) {
        seq_cache_max = 0;
        seq_cache_len = 1;
        seq_cache = mem_new(seq_t, 1);
        if (!seq_cache) exit(EXIT_FAILURE);
        seq_cache[0] = (seq_t)3;  /* a(0) = 3 */
    }

    if (n <= seq_cache_max)          /* already computed */
        return seq_cache[n];

    /* Ensure prime list contains at least n primes (since n > 0) */
    if (n > 0)
        ensure_primes((size_t)n);

    /* Grow sequence cache as needed */
    ensure_seq_cache(n);

    /* Compute new terms */
    for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
        idx_t  prime_idx = i;          /* 1‑based index for p_i */
        uint64_t pk = prime_cache[prime_idx - 1]; /* p_i */
        seq_t prev = seq_cache[i - 1];
        seq_cache[i] = find_smallest_prime(prev, pk);
    }

    seq_cache_max = n;
    return seq_cache[n];
}
