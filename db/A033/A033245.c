/*
 * A033245  Primes of form x^2+70*y^2.
 * https://oeis.org/A033245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes of the form x^2 + 70*y^2
   (OEIS A033245)
   Computation: sieve the integers up to a growing bound and keep the
   primes that are congruent to one of the 12 allowed residues modulo
   280 (see the OEIS entry).  Results are cached, so any order of calls
   is handled efficiently.  The routine is O(n) for the n‑th term. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Residues modulo 280 for primes of the form x^2 + 70*y^2. */
static const int residues[12] = {
    1, 9, 39, 71, 79, 81, 121, 151, 169, 191, 239, 249
};

/*--- Prime check for small primes (≤ 2^32) --------------------------------*/
static inline bool is_allowed_residue(uint32_t p)
{
    uint32_t r = p % 280;
    for (int i = 0; i < 12; ++i) {
        if (residues[i] == r) return true;
    }
    return false;
}

/*--- Sieve infrastructure (globally static) --------------------------------*/
static uint8_t *sieve_composite = NULL;   /* 0 = prime, 1 = composite */
static idx_t   sieve_limit = 0;            /* current maximal sieve bound */
static idx_t   last_checked = 1;           /* last integer processed */

/* Allocate and run a sieve up to 'new_limit' (inclusive). */
static void rebuild_sieve(idx_t new_limit)
{
    /* Allocate new table; +1 for index=new_limit. */
    size_t size = (size_t)new_limit + 1;
    uint8_t *new_composite = (uint8_t *)calloc(size, sizeof(uint8_t));
    if (!new_composite) return; /* allocation failure – not expected */

    new_composite[0] = 1;
    if (new_limit > 0) new_composite[1] = 1;

    idx_t i, j;
    for (i = 2; i * i <= new_limit; ++i) {
        if (!new_composite[i]) {
            for (j = i * i; j <= new_limit; j += i) {
                new_composite[j] = 1;
            }
        }
    }

    free(sieve_composite);
    sieve_composite = new_composite;
    sieve_limit = new_limit;
}

/*--- Cache of sequenced terms -------------------------------------------*/
#define CACHE_CAPACITY 100000 /* sufficient for n up to several 10^4 */
static seq_t cache[CACHE_CAPACITY];
static idx_t cache_count = 0;

/*--- Main routine -------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n <= cache_count && n > 0) {
        return cache[n - 1];
    }

    /* Initialise sieve if needed. */
    if (sieve_limit == 0) {
        rebuild_sieve(100000);           /* modest starting bound */
    }

    /* Grow cache capacity if necessary (defensive). */
    if (cache_count + 1 > CACHE_CAPACITY) {
        return 0; /* overflow – should not happen for n ≤ 10000 */
    }

    /* Find primes until we reach the n‑th desired term. */
    while (cache_count < n) {
        /* Extend sieve if we have reached the limit. */
        if (last_checked >= sieve_limit) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit <= sieve_limit) new_limit = sieve_limit + 1000;
            rebuild_sieve(new_limit);
        }

        /* Scan integers beyond the last checked one. */
        idx_t i;
        for (i = last_checked + 1;
             i <= sieve_limit && cache_count < n;
             ++i)
        {
            if (!sieve_composite[i] && is_allowed_residue((uint32_t)i)) {
                cache[cache_count++] = (seq_t)i;
            }
        }
        last_checked = i - 1; /* remember where we stopped */
    }

    return cache[n - 1];
}
