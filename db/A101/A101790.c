/*
 * A101790  Numbers k such that 4*k-1, 8*k-1 and 16*k-1 are all primes.
 * https://oeis.org/A101790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9420 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101790.
   A(n) is the n‑th integer k such that 4k-1, 8k-1 and 16k-1 are all
   prime.  The algorithm iterates over primes p=4k-1, and for each
   p tests that 2p+1 and 4p+3 are also prime.  The value k=(p+1)/4 is
   recorded.  Results are cached so that subsequent calls for the same
   or larger indices are O(1).  The implementation uses only the
   supplied helpers and is valid C11/C++17.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

static seq_t *cache = NULL;     /* cached sequence values */
static idx_t  cache_cap = 0;    /* allocated capacity of cache */
static idx_t  cache_size = 0;   /* number of elements stored */
static uint64_t next_candidate = 3;  /* the next odd integer to test */

seq_t a(idx_t n)
{
    /* Offset of A101790 is 1 – n=1 is the first term.  */
    if (n == 0) return 0;          /* undefined, but guard against zero index */

    /* Generate terms until we have at least n of them.  */
    while (cache_size < n) {
        /* Find the next prime p.  */
        while (!is_prime_u64(next_candidate))
            next_candidate += 2;      /* only odd candidates */

        uint64_t p = next_candidate;

        /* Check the additional primality conditions.  */
        if ((p & 3U) == 3U &&
            is_prime_u64(2ULL * p + 1ULL) &&
            is_prime_u64(4ULL * p + 3ULL)) {

            seq_t k = ((seq_t)p + 1) / 4;   /* k = (p+1)/4 */
            push_seq(&cache, &cache_cap, &cache_size, k);
        }

        next_candidate += 2;          /* advance to the next odd integer */
    }

    return cache[n - 1];               /* 1‑based indexing */
}
