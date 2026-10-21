/*
 * A098423  Primes occurring in exactly three prime triples (p,q,r) with
 *          p<q<r=p+6.
 * https://oeis.org/A098423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   build_cache_up_to, find_prime_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A098423: primes occurring in exactly three prime triples
 * (p,q,r) with p<q<r=p+6. Computed by sieving primes up to a limit
 * and counting how often each prime appears in such triples. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_cache_up_to.h"
#include "../utils/find_prime_index.h"

/* Cache for the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static idx_t  current_limit = 0;

/* External function required by the evaluator */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset is 1 */
    if (n <= seq_len)                          /* already cached */
        return seq_cache[n - 1];

    /* need more terms: rebuild with larger limits */
    idx_t limit = 100000;                      /* starting sieve size */
    seq_t *new_cache = NULL;
    size_t new_len = 0;
    do {
        /* free previous attempt if any */
        if (new_cache) free(new_cache);

        build_cache_up_to(limit, &new_len, &new_cache);
        limit <<= 1;              /* double for next trial if necessary */
    } while (new_len < (size_t)n);

    /* replace old cache */
    if (seq_cache) free(seq_cache);
    seq_cache = new_cache;
    seq_len  = new_len;
    current_limit = limit >> 1;   /* remember last used limit */

    return seq_cache[n - 1];
}
