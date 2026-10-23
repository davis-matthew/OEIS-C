/*
 * A140018  Primes of the form 7x^2+195y^2.
 * https://oeis.org/A140018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A140018: primes of the form 7x^2+195y^2.
 * The primes are exactly those congruent to one of the 38 residues
 * listed in the OEIS entry: {7,187,223,307,643,787,943,1123,1207,1363,
 * 1567,1627,1783,1867,1903,1987,2047,2203,2407,2803,2827,3127,3307,
 * 3463,3547,3583,3967,4063,4087,4387,4483,4843,4903,4987,5143,5263,5323}
 * modulo the discriminant 5460. The function generates primes via a
 * simple incremental sieve and collects those with a matching residue.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

static seq_t       *cache = NULL;          /* discovered values   */
static size_t       cache_cap = 0;          /* allocated capacity  */
static idx_t        cache_len = 0;          /* number of stored terms */

static uint64_t     last_checked = 2;       /* next integer to test */
static uint8_t     *sieve = NULL;           /* 0 = prime, 1 = composite */
static uint64_t     sieve_limit = 0;        /* highest number sieved */

static unsigned char good_mod[5460] = {0};  /* residue table        */
static int          good_mod_inited = 0;    /* init flag            */

static void extend_sieve(uint64_t limit);

seq_t a(idx_t n)
{
    /* Initialise residue table once. */
    if (!good_mod_inited) {
        const uint16_t residues[] = {
            7,187,223,307,643,787,943,1123,1207,1363,
            1567,1627,1783,1867,1903,1987,2047,2203,
            2407,2803,2827,3127,3307,3463,3547,3583,
            3967,4063,4087,4387,4483,4843,4903,4987,
            5143,5263,5323
        };
        size_t i;
        for (i = 0; i < sizeof(residues)/sizeof(residues[0]); ++i)
            good_mod[ residues[i] % 5460 ] = 1;
        good_mod_inited = 1;
    }

    /* Ensure we have at least n values. */
    while (cache_len < n) {
        /* If the sieve is too small, extend it.  The multiplier 2000
           gives a generous cushion for the first 10 000 terms (the
           10 000‑th prime of this form is < 2×10^7). */
        if (sieve_limit < last_checked) {
            extend_sieve(sieve_limit + 100000);  /* small jump first */
        }

        if (sieve_limit < last_checked + 10000) {
            extend_sieve(sieve_limit + 10000);
        }

        /* Scan primes newly sieved. */
        for ( ; last_checked <= sieve_limit && cache_len < n; ++last_checked ) {
            if (!sieve[(size_t)last_checked]) {                    /* prime */
                if (good_mod[ (uint64_t)last_checked % 5460 ]) {
                    /* Grow cache if needed. */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                        cache = seq_realloc(cache, cache_cap, new_cap);
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)last_checked;
                }
            }
        }
    }
    return cache[n-1];
}

static void extend_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;             /* already enough */

    /* Reallocate sieve array.  The cast is required for C++. */
    uint8_t *new_sieve = (uint8_t *)realloc(sieve, (limit + 1) * sizeof(uint8_t));
    if (!new_sieve) abort();                      /* unrecoverable failure */
    sieve = new_sieve;

    /* Mark 0 and 1 as composite. */
    sieve[0] = 1;
    if (limit >= 1) sieve[1] = 1;

    /* Sieve of Eratosthenes over the new range. */
    uint64_t p;
    for (p = 2; p * p <= limit; ++p) {
        if (!sieve[(size_t)p]) {                  /* p is prime */
            uint64_t start = p * p;
            if (start < sieve_limit + 1) {
                start = ((sieve_limit + 1 + p - 1) / p) * p;
                if (start < p * p) start = p * p;
            }
            for (uint64_t m = start; m <= limit; m += p)
                sieve[(size_t)m] = 1;
        }
    }
    sieve_limit = limit;
}
