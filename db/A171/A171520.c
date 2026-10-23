/*
 * A171520  Integers m that are not the product of k-th prime and k for any k
 *          >= 1.
 * https://oeis.org/A171520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A171520: integers that are NOT of the form p_k * k for any k ≥ 1.
   The function builds a sieve up to a dynamic bound, stores the
   complement sequence in a static cache, and expands it only when
   needed. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache holds the complement sequence */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0)   /* offset starts at 1; undefined for n==0 */
        return 0;

    /* If we already have enough terms, return the requested one */
    if (cache_len >= n)
        return cache[n - 1];

    /* -------------------------------------------------------------
       Expand the cache until we have at least n terms.
       ------------------------------------------------------------- */
    idx_t needed = n;
    /* initial search bound */
    idx_t limit = cache_len > (idx_t)10 ? cache_len : needed * 3 + 10;

    while (1) {
        /* ---- 1. Get primes up to the current limit. --------- */
        uint64_t *prime_arr = NULL;
        size_t    prime_cnt = 0;
        build_primes_upto(limit, &prime_arr, &prime_cnt);

        /* ---- 2. Mark all numbers of the form p_k * k. -------- */
        size_t mem_lim = (size_t)limit + 1;
        unsigned char *excluded = (unsigned char *)calloc(mem_lim, 1);
        if (!excluded) {
            free(prime_arr);
            return 0;
        }

        for (size_t k = 0; k < prime_cnt; ++k) {
            seq_t val = (seq_t)prime_arr[k] * (seq_t)(k + 1);
            if (val <= (seq_t)limit) {
                excluded[(size_t)val] = 1;
            } else {
                /* primes are increasing, further values will be larger */
                break;
            }
        }

        /* ---- 3. Collect all non‑excluded numbers. ------------ */
        seq_t *tmp_cache = (seq_t *)malloc(sizeof(seq_t) * mem_lim);
        if (!tmp_cache) {
            free(excluded);
            free(prime_arr);
            return 0;
        }

        idx_t collected = 0;
        for (idx_t m = 1; m <= (idx_t)limit; ++m) {
            if (!excluded[(size_t)m]) {
                tmp_cache[collected++] = (seq_t)m;
            }
        }

        free(excluded);
        free(prime_arr);

        /* Did we reach the required number of terms? */
        if (collected >= needed) {
            /* Replace old cache with the new one */
            free(cache);
            cache      = tmp_cache;
            cache_len  = collected;
            break;  /* cache is ready */
        }

        /* Not enough terms; discard and try again with a larger bound */
        free(tmp_cache);
        limit *= 2;   /* grow limit exponentially to keep O(n) overall */
    }

    /* cache_len >= n now */
    return cache[n - 1];
}
