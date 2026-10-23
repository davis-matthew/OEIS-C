/*
 * A101790  Numbers k such that 4*k-1, 8*k-1 and 16*k-1 are all primes.
 * https://oeis.org/A101790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, push_seq
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
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

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
        while (!is_prime(next_candidate))
            next_candidate += 2;      /* only odd candidates */

        uint64_t p = next_candidate;

        /* Check the additional primality conditions.  */
        if ((p & 3U) == 3U &&
            is_prime(2ULL * p + 1ULL) &&
            is_prime(4ULL * p + 3ULL)) {

            seq_t k = ((seq_t)p + 1) / 4;   /* k = (p+1)/4 */
            push_seq(&cache, &cache_cap, &cache_size, k);
        }

        next_candidate += 2;          /* advance to the next odd integer */
    }

    return cache[n - 1];               /* 1‑based indexing */
}
