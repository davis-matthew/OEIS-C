/*
 * A141026  Numbers k such that (k,k+8) forms a pair of consecutive primes
 *          ending respectively in 1 and 9.
 * https://oeis.org/A141026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates terms of OEIS A141026:
   k such that (k, k+8) is a pair of consecutive primes ending
   respectively in 1 and 9.  k itself ends with 1 and k+8 ends with 9. */
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/grow_seq.h"

static seq_t *cache          = NULL;   /* dynamically grown array of terms   */
static size_t cache_len      = 0;      /* number of stored terms              */
static size_t cache_cap      = 0;      /* current capacity of cache           */
static uint64_t last_prime   = 3;      /* last prime examined (next to start) */

/* Locate the nth element (1‑based) in A141026.  */
seq_t a(idx_t n)
{
    if (n < 1)                        /* undefined; return 0 for safety  */
        return (seq_t)0;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t p = last_prime;
        uint64_t q = next_prime(p);   /* next prime after p   */

        /* Consider pair (p, q). If p ends in 1, q will end in 9 automatically
           if the gap is exactly 8.  We only accept consecutive primes
           with that gap. */
        if ((p % 10) == 1L && (q - p) == 8ULL) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = (seq_t)p;
            cache_len++;
        }

        /* Advance to the next prime for the next iteration. */
        last_prime = q;
    }
    return cache[n-1];
}
