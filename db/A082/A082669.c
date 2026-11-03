/*
 * A082669  a(n) = p(n)*(p(n)-1)/2 where p(n) = upper member of n-th pair of
 *          twin primes.
 * https://oeis.org/A082669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A006512(n)*(A006512(n)-1)/2
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    where A006512(n) is the upper member of the n-th twin‑prime pair. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static seq_t *twin_cache = NULL;        /* 1‑based upper twin primes */
static idx_t  twin_cache_used = 0;
static size_t twin_cache_cap   = 0;
static uint64_t twin_candidate = 3;      /* next odd number to test */

/* Grow the cache until it contains at least “target” twin primes. */
static void grow_twin_cache_to(idx_t target)
{
    if (target <= twin_cache_used)
        return;

    /* Allocate more cache space if necessary. */
    if (twin_cache_cap < (size_t)target + 1) {     /* 1-based: index target is used */
        twin_cache_cap = target * 2 + 10;          /* buffer */
        seq_t *new_cache = mem_new(seq_t, twin_cache_cap);
        if (!new_cache)
            exit(EXIT_FAILURE);
        if (twin_cache) {
            mem_copy(new_cache, twin_cache, (twin_cache_used + 1) * sizeof(seq_t));
            mem_free(twin_cache);
        }
        twin_cache = new_cache;
    }

    /* Find successive twin primes. */
    while (twin_cache_used < target) {
        uint64_t lower = twin_candidate;
        twin_candidate += 2;                      /* only odd candidates */
        if (is_prime_seq(lower) && is_prime_seq(lower + 2)) {
            twin_cache[twin_cache_used + 1] = (seq_t)(lower + 2);  /* 1‑based */
            ++twin_cache_used;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 */
        return (seq_t)0;

    grow_twin_cache_to(n);

    seq_t upper = twin_cache[n];

    /* Compute upper*(upper-1)/2 while avoiding overflow. */
    seq_t result;
    if ((upper & (seq_t)1) == 0) {           /* upper even */
        result = (upper >> 1) * (upper - (seq_t)1);
    } else {                                 /* upper odd */
        result = upper * ((upper - (seq_t)1) >> 1);
    }
    return result;
}
