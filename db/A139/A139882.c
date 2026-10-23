/*
 * A139882  Primes of the form 3x^2+59y^2.
 * https://oeis.org/A139882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A139882
 *  Primes of the form 3*x^2 + 59*y^2
 *  Implementation builds a simple prime sieve up to an increasing
 *  limit and collects primes that satisfy the congruence condition
 *  modulo 708.  Results are cached so that calls with arbitrary n
 *  order remain efficient. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* --- residues that a prime of the form 3*x^2+59*y^2 may have modulo 708 --- */
static const uint16_t residues_list[] = {
    3, 35, 59, 71, 95, 107, 119, 143, 167, 203,
    239, 251, 263, 287, 299, 311, 323, 359, 371, 383,
    395, 407, 479, 491, 551, 599, 611, 635, 647, 671,
    695
};
static const size_t residues_cnt = sizeof(residues_list) / sizeof(residues_list[0]);

static bool in_residues(uint64_t m)
{
    for (size_t i = 0; i < residues_cnt; ++i)
        if (m == residues_list[i])
            return true;
    return false;
}

/* --- prime sieve data ---------------------------------------------- */
static unsigned char *sieve = NULL;      /* bit array: bit i == 1 iff composite */
static uint64_t sieve_limit = 0;         /* largest number sieved */

/* Allocate and build sieve up to at least 'limit' (inclusive). */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;

    uint64_t new_limit = limit;
    size_t bytes = (new_limit >> 3) + 1;
    unsigned char *new_sieve = (unsigned char *)malloc(bytes);
    if (!new_sieve)
        abort();
    memset(new_sieve, 0, bytes);

    /* Mark 0 and 1 as composite. */
    new_sieve[0] |= 1;          /* 0 */
    new_sieve[0] |= 1 << 1;     /* 1 */

    uint64_t sqrt_limit = (uint64_t)sqrt((double)new_limit);
    for (uint64_t p = 2; p <= sqrt_limit; ++p) {
        if ((new_sieve[p >> 3] >> (p & 7)) & 1)
            continue;          /* composite */
        for (uint64_t mult = p * p; mult <= new_limit; mult += p)
            new_sieve[mult >> 3] |= 1 << (mult & 7);
    }

    /* Replace old sieve. */
    free(sieve);
    sieve = new_sieve;
    sieve_limit = new_limit;
}

/* --- cache of primes of the required form --------------------------- */
static seq_t *cache = NULL;    /* array of primes of the form */
static size_t cache_len = 0;   /* number of primes collected */
static size_t cache_cap = 0;   /* allocated capacity */

/* Last integer examined; we continue from this + 1. */
static uint64_t last_checked = 2;

/* Expand cache until it contains at least 'target' entries. */
static void fill_cache_until(size_t target)
{
    if (cache_len >= target)
        return;

    /* Ensure cache array large enough. */
    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < target)
            new_cap = target;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp)
            abort();
        cache = newp;
        cache_cap = new_cap;
    }

    /* Search for more primes in increasing order.  The sieve limit is
       grown geometrically until enough primes are found. */
    uint64_t limit = last_checked + 1000;   /* start with a moderate size */
    while (cache_len < target) {
        /* Ensure sieve covers up to 'limit'. */
        ensure_sieve(limit);

        /* Scan candidates from last_checked+1 up to 'limit'. */
        for (uint64_t n = last_checked + 1; n <= limit; ++n) {
            /* Quick skip: even numbers > 2 are composite. */
            if (n > 2 && (n & 1) == 0)
                continue;

            /* Test primality via sieve. */
            if ((sieve[n >> 3] >> (n & 7)) & 1)        /* composite */
                continue;

            /* Prime -> check residue condition. */
            if (in_residues(n % 708)) {
                cache[cache_len++] = (seq_t)n;
                if (cache_len >= target) {
                    last_checked = n;   /* next search starts after this. */
                    return;
                }
            }
        }

        /* Not enough yet: extend limit. */
        last_checked = limit;
        limit *= 2;
    }
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1, return 0 as placeholder */

    /* Convert idx_t to size_t safely; idx_t is unsigned 64‑bit. */
    size_t target = (size_t)n;

    /* Populate cache if necessary. */
    fill_cache_until(target);

    return cache[n - 1];
}
