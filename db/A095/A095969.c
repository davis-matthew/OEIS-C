/*
 * A095969  If p(k) is the k-th prime, then the n-th set of 2 consecutive
 *          cousin prime pairs starts at p(a(n)).
 * https://oeis.org/A095969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the prime that starts the nth set of two
 *            consecutive cousin prime pairs. This function searches primes
 *            sequentially, detects cousin pair groups, and memoises the
 *            starting indices
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/*--- Prime storage -------------------------------------------*/
static uint64_t *prime_list = NULL;   /* list of found primes */
static size_t    prime_cnt  = 0;       /* number of primes stored     */
static size_t    prime_cap  = 0;       /* current allocation capacity */

/* Grow the prime array when needed (trivial type, so realloc is fine). */
static inline void grow_prime(void)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_list = (uint64_t *)realloc(prime_list,
                                         prime_cap * sizeof(uint64_t));
    }
}

/* Next odd integer to test; 2 is handled specially. */
static uint64_t next_candidate = 2;

/* Test primality using trial division by the primes discovered so far. */
static inline bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p) break;        /* p*p > n */
        if (n % p == 0) return false;
    }
    return true;
}

/*--- Cache for a(n) values ------------------------------------*/
static seq_t *seq_cache = NULL;   /* memoised a(n) values (1‑based) */
static size_t  seq_cnt   = 0;     /* number of cached terms          */
static size_t  seq_cap   = 0;     /* capacity of cache array          */

/* Ensure the cache can hold at least 'needed' elements.
   For user‑defined big–int types we allocate a new array and copy the
   contents, because realloc may not be safe for non‑trivial types. */
static void ensure_cache_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    seq_t *new_cache = alloc_seq_array(new_cap);
    for (size_t i = 0; i < seq_cnt; ++i) new_cache[i] = seq_cache[i];
    free_seq_array(seq_cache);
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

/*--- Main sequence function ------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset 1, n=0 is unused */

    /* Generate primes and detect cousin‑pair groups until we have a(n). */
    while (seq_cnt < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Advance to next candidate: 2,3,5,7,9,... */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;

        if (!is_prime(cand))
            continue;          /* not a prime */

        grow_prime();
        prime_list[prime_cnt++] = cand;

        /* When at least 4 primes are known, check the last four. */
        if (prime_cnt >= 4) {
            uint64_t p0 = prime_list[prime_cnt - 4];
            uint64_t p1 = prime_list[prime_cnt - 3];
            uint64_t p2 = prime_list[prime_cnt - 2];
            uint64_t p3 = prime_list[prime_cnt - 1];

            /* p0,p1 and p2,p3 are cousin pairs and p1,p2 are consecutive primes. */
            if (p0 + 4 == p1 && p2 + 4 == p3) {
                ensure_cache_cap(seq_cnt + 1);
                seq_cache[seq_cnt++] = (seq_t)(prime_cnt - 3); /* 1‑based index of p0 */
            }
        }
    }

    return seq_cache[n - 1];
}
