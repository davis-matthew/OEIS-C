/*
 * A106956  Primes of the form 4x^2+xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 612, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_ui32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculate Primes of the form 4·x² + x·y + 5·y² (x,y ≥ 0).
   The result is the n‑th such prime (n≥1).  A sieve of Eratosthenes
   is used to test primality.  The list of candidate numbers is
   generated, sorted and unique, then filtered by the prime mask.  The
   implementation uses only standard C features and works under C11
   and C++17.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/compare_ui32.h"

/* Compute the n‑th prime of the form 4*x² + x*y + 5*y². */
seq_t a(idx_t n)
{
    /* Static storage to keep the data between calls. */
    static uint64_t built_limit = 0;          /* current sieve limit      */
    static uint8_t *prime_mask = NULL;        /* sieve bit‑array          */
    static uint32_t *candidates = NULL;       /* all values <= limit      */
    static size_t cand_size = 0, cand_used = 0;/* array capacity and count */
    static seq_t *prime_list = NULL;          /* list of primes found     */
    static size_t prime_cap = 0, prime_cnt = 0;/* capacity and number of primes cached */

    /* Estimate required sieve limit.  70*n is a generous guess for the
       density of the required primes; the hard cap 5 M keeps the memory
       usage modest. */
    uint64_t estimate_limit = (uint64_t)n * 70ULL + 100ULL;
    uint64_t limit = estimate_limit > 5000000ULL ? estimate_limit : 5000000ULL;

    if (limit > built_limit) {
        /* Re‑sieve up to the new limit. */
        uint64_t i, j;

        /* Allocate or reallocate the prime sieve mask. */
        if (prime_mask) free(prime_mask);
        prime_mask = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
        if (!prime_mask) abort(); /* memory failure */

        /* Simple Eratosthenes sieve. */
        prime_mask[0] = prime_mask[1] = 0;
        for (i = 2; i <= limit; ++i) prime_mask[i] = 1;
        uint64_t sq = (uint64_t)sqrt((double)limit);
        for (i = 2; i <= sq; ++i) {
            if (prime_mask[i]) {
                for (j = i * i; j <= limit; j += i)
                    prime_mask[j] = 0;
            }
        }

        /* Resize candidate array if necessary.  Estimated size:
           number of pairs with 4*x² + x*y + 5*y² ≤ limit.  Roughly
           limit/(4+5)=limit/9 per (x,y) grid.  Allocate plenty. */
        size_t new_cap = (size_t)(limit / 2); /* conservative */
        if (candidates == NULL || cand_size < new_cap) {
            if (candidates) free(candidates);
            candidates = (uint32_t *)malloc(new_cap * sizeof(uint32_t));
            if (!candidates) abort();
            cand_size = new_cap;
        }
        cand_used = 0;

        /* Generate all values of the form 4x² + xy + 5y² ≤ limit. */
        for (uint32_t y = 0; ; ++y) {
            uint64_t base = 5ULL * y * y;
            if (base > limit) break;
            for (uint32_t x = 0; ; ++x) {
                uint64_t val = 4ULL * x * x + (uint64_t)x * y + base;
                if (val > limit) break;
                if (cand_used == cand_size) {
                    /* enlarge array */
                    size_t newsize = cand_size * 2;
                    uint32_t *tmp = (uint32_t *)realloc(candidates, newsize * sizeof(uint32_t));
                    if (!tmp) abort();
                    candidates = tmp;
                    cand_size = newsize;
                }
                candidates[cand_used++] = (uint32_t)val;
            }
        }

        /* Sort candidates and remove duplicates. */
        qsort(candidates, cand_used, sizeof(uint32_t), compare_ui32);
        size_t unique_cnt = 0;
        for (size_t i = 0; i < cand_used; ++i) {
            if (i == 0 || candidates[i] != candidates[i - 1])
                candidates[unique_cnt++] = candidates[i];
        }
        cand_used = unique_cnt;

        /* Build the prime list from the unique candidates. */
        if (prime_list) free(prime_list);
        prime_cap = (size_t)(n > 10000 ? n : 10000); /* minimum capacity */
        prime_list = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (!prime_list) abort();
        prime_cnt = 0;
        for (size_t i = 0; i < cand_used && prime_cnt < prime_cap; ++i) {
            uint32_t v = candidates[i];
            if (prime_mask[v]) {
                prime_list[prime_cnt++] = (seq_t)v;
            }
        }

        /* Update the built limit. */
        built_limit = limit;
    }

    /* If more primes are required than cached, increase the sieve limit. */
    if (prime_cnt < n) {
        /* Double the limit and rebuild. */
        limit = built_limit * 2;
        /* Recurse after rebuilding. */
        return a(n);
    }

    /* n is 1‑based; access the (n‑1)th element. */
    return prime_list[n - 1];
}
