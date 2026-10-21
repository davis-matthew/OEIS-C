/*
 * A063912  Primes p such that 2*p - 11 is also prime.
 * https://oeis.org/A063912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 565, u32 n <= 331, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big, seq_alloc_new_array,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A063912: primes p such that 2*p–11 is also prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"          /* bool is_prime_big(seq_t n) */
#include "../utils/build_primes_upto.h"     /* build_primes_upto(limit, &primes, &cnt) */
#include "../utils/seq_alloc_new_array.h"  /* seq_alloc_new_array(n) */
#include "../utils/seq_realloc.h"          /* seq_realloc(ptr, old_cap, new_cap) */

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
