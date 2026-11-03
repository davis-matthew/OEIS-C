/*
 * A063912  Primes p such that 2*p - 11 is also prime.
 * https://oeis.org/A063912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 565, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 977 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A063912: primes p such that 2*p–11 is also prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

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

/* Static cache of sequence values */
static seq_t *value_cache   = NULL;
static size_t value_len     = 0;
static size_t value_cap     = 0;

/* Primes up to current limit */
static uint64_t *prime_list        = NULL;
static size_t     prime_cnt        = 0;
static size_t     prime_processed  = 0;   /* index of next prime to examine */
static idx_t      prime_limit      = 0;   /* maximum value sieved */

/* Grow the value cache when needed */
static void ensure_value_cap(size_t needed)
{
    if (needed > value_cap) {
        size_t new_cap = value_cap ? value_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = seq_realloc(value_cache, value_cap, new_cap);
        if (!tmp) abort();  /* out of memory */
        value_cache = tmp;
        value_cap   = new_cap;
    }
}

/* Append a value to the cache */
static void push_value(seq_t v)
{
    ensure_value_cap(value_len + 1);
    value_cache[value_len++] = v;
}

/* Extend prime sieve to at least new_limit and process new primes */
static void extend_to(idx_t new_limit)
{
    /* Allocate new prime list */
    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    build_primes_upto(new_limit, &new_list, &new_cnt);

    /* Replace the old list */
    free(prime_list);
    prime_list   = new_list;
    prime_cnt    = new_cnt;
    prime_limit  = new_limit;

    /* Check new primes for the condition */
    for (size_t i = prime_processed; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t val2 = (seq_t)( ( (seq_t)p * (seq_t)2 ) - (seq_t)11 );
        if (val2 >= (seq_t)2 && is_prime_big(val2))
            push_value( (seq_t)p );
    }
    prime_processed = prime_cnt;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the specification                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined behaviour below offset */

    /* Ensure we have computed at least n values */
    while (value_len < (size_t)n) {
        /* Choose next sieve limit (double each time, start at 1000) */
        idx_t next_limit = prime_limit ? prime_limit * 2 : 1000;
        extend_to(next_limit);
    }
    return value_cache[n - 1];
}
