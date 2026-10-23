/*
 * A186193  Numbers k such that k!! is divisible by (k+1).
 * https://oeis.org/A186193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th even integer k ≥ 14 such that k+1 is composite
 * Range:     u8 n <= 73, u16 n <= 697, u32 n <= 697, u64 n <= 697,
 *            u128 n <= 697, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, all even k ≥ 14 with k+1 not prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* static cache for the generated terms */
static seq_t *cache      = NULL;   /* array of terms */
static idx_t cache_cnt   = 0;      /* number of stored terms          */
static size_t cache_cap  = 0;      /* allocated capacity              */
static idx_t next_even   = 14;     /* next even candidate to test    */

/* grow the cache so that at least `need` more slots are available */
static void ensure_cache_capacity(size_t need)
{
    size_t required = cache_cnt + need;
    if (required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < required) new_cap *= 2;
    /* cast the result of realloc to the appropriate type */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE); /* memory exhausted */
    cache      = new_ptr;
    cache_cap  = new_cap;
}

/* check if a number is composite (i.e., not prime) */
static bool is_composite(seq_t x) { return !isPrimeGeneric(x); }

/* a(idx_t n): return the n-th term (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; return 0 safely */

    /* generate terms until we have at least n terms */
    while (cache_cnt < n) {
        seq_t kp1 = (seq_t)next_even + 1ULL;   /* k+1 */
        if (is_composite(kp1)) {
            ensure_cache_capacity(1);
            cache[cache_cnt++] = (seq_t)next_even;
        }
        next_even += 2ULL;          /* next even candidate */
    }
    return cache[n - 1];
}
