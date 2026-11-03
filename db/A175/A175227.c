/*
 * A175227  Sequence of the remaining nonprimes out of sieve of type: {step 1:
 *          starting with the sequence of positive integers A000027; step 2:
 *          delete every prime number A000040; step 3: delete every
 *          prime(1)-th number greater than the prime(1) from the remaining
 *          sequence; step 4: delete every prime(2)-th nu...
 * https://oeis.org/A175227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A175227 (OEIS)
   Sequence of remaining nonprimes after progressively deleting every
   p‑th element greater than p for each prime p (p = 2,3,5,7,…).  The
   implementation builds the list of surviving numbers up to a dynamic
   upper bound until it contains at least the requested term index. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *
compute_sequence(idx_t limit, size_t *out_len)
{
    /* Allocate an array big enough for all integers up to 'limit' */
    size_t cap = (size_t)limit + 1;
    seq_t *seq = mem_new(seq_t, cap);
    if (!seq) return NULL; /* allocation failure */

    /* Sieve of Eratosthenes for primes up to 'limit' */
    bool *isPrime = (bool *)calloc(cap, sizeof(bool));
    if (!isPrime) { mem_free(seq); return NULL; }
    for (size_t i = 2; i < cap; ++i) isPrime[i] = true;
    for (size_t i = 2; i * i < cap; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j < cap; j += i) isPrime[j] = false;
        }
    }
    isPrime[0] = isPrime[1] = false;

    /* Initial list: all non‑primes from 1 to limit */
    size_t sz = 0;
    for (size_t i = 1; i < cap; ++i) {
        if (!isPrime[i]) seq[sz++] = (seq_t)i;
    }

    /* Iteratively apply deletions for each prime p <= limit */
    for (size_t p = 2; p < cap; ++p) {
        if (!isPrime[p]) continue;               /* skip non-primes */

        size_t counter = 0;          /* count of elements > p */
        size_t dest = 0;             /* write position */

        for (size_t i = 0; i < sz; ++i) {
            seq_t val = seq[i];
            if (val <= (seq_t)p) {
                seq[dest++] = val;
            } else {
                ++counter;
                if (counter % p != 0) seq[dest++] = val;
            }
        }
        sz = dest;                    /* shrink list after this step */
    }

    free(isPrime);
    *out_len = sz;
    return seq;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached survivors */
    static size_t cache_len = 0;
    static idx_t cache_limit = 0;

    if (n == 0) return (seq_t)0;   /* undefined for 0 but safe */

    /* Rebuild sequence if we don't yet have enough terms */
    if (cache_len < (size_t)n) {
        idx_t limit = cache_limit ? cache_limit
                                  : ((n > 100) ? n * 10 : 100);
        if (limit < n) limit = n * 10;

        while (true) {
            size_t len;
            seq_t *newc = compute_sequence(limit, &len);
            if (!newc) return (seq_t)0;   /* allocation failure */

            if (cache) mem_free(cache);
            cache = newc;
            cache_len = len;
            cache_limit = limit;

            if (cache_len >= (size_t)n) break;
            limit *= 2;                    /* increase bound and retry */
        }
    }

    return cache[n - 1];
}
