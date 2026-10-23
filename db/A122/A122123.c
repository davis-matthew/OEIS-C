/*
 * A122123  Product of the first n 5-almost primes (A014614).
 * https://oeis.org/A122123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Prod[i=1..n] A014614(i) (product of first n 5‑almost
 *            primes)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"   /* build_spf */

/* static state: SPF table and its current size (max index it covers) */
static idx_t *spf_table = NULL;
static idx_t spf_limit = 0;

/* Ensure the SPF table covers every integer up to `limit`.  If a larger
   table is needed, free the old one and build a new one.  The table
   is an array of size `limit + 1` with spf_table[x] = smallest prime
   factor of x for all x >= 2.  The element for 0 and 1 is unused. */
static void ensure_spf_limit(idx_t limit)
{
    if (limit <= spf_limit)
        return;                      /* already sufficient */

    free(spf_table);                  /* free previous table */
    build_spf(limit, &spf_table);     /* build new table */
    spf_limit = limit;
}

/* Count the total number of prime factors (with multiplicity) of `x`
   using the precomputed SPF table.  x > 1. */
static idx_t count_omega(uint64_t x)
{
    idx_t omega = 0;
    while (x > 1) {
        uint64_t p = spf_table[x];
        x /= p;
        ++omega;
    }
    return omega;
}

/* Return the n‑th term of the sequence.  For n >= 1,
   the product of the first n 5‑almost primes is computed.
   Calls with arbitrary n order are supported via memoisation
   of the SPF table; the algorithm is O(n log n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    seq_t prod = 1;                   /* product accumulator */
    idx_t seen = 0;                   /* number of 5‑almost primes found */

    /* start with a small sieve limit and grow as needed */
    idx_t limit = 2;
    ensure_spf_limit(limit);

    /* iterate integers starting from 2 */
    idx_t i = 2;
    while (seen < n) {
        /* If we exceed the current sieve limit, double it. */
        if (i > limit) {
            idx_t new_limit = limit * 2;
            /* guard against overflow (unlikely for the sizes needed) */
            if (new_limit <= limit)
                new_limit = limit + 1;
            limit = new_limit;
            ensure_spf_limit(limit);
        }

        /* Count ω(i) */
        if (count_omega((uint64_t)i) == 5) {
            prod *= (seq_t)i;         /* multiply into product */
            ++seen;
            if (seen == n)
                return prod;          /* done */
        }
        ++i;
    }

    return prod;                      /* should never reach here */
}
