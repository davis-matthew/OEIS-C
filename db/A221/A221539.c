/*
 * A221539  Number of 0..5 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A221539: Number of 0..5 arrays of length n with each element
   differing from at least one neighbor by something other than 1,
   starting with 0.  (Offset 1)
   Empirical recurrence:
     a(n) = 5*a(n-1) + 3*a(n-2) + 9*a(n-4) + 6*a(n-5) + 3*a(n-6)
   Initial values:
     a(1)=0, a(2)=5, a(3)=22, a(4)=130, a(5)=716, a(6)=4018
   The function caches computed terms so that calls in any order
   remain O(max n).

   The implementation uses a static array that is grown on demand.
   No dynamic memory is freed, because this is a typical library
   helper for sequence generation.  The code is valid C11/C++17. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cached values, 1‑based index, cache[0] unused   */
static size_t cache_len = 0;  /* allocated length of cache (including unused 0) */
static idx_t  max_calculated = 0;  /* largest n already computed  */

/* Return a(n) for n >= 1.  Behaviour for n < 1 is undefined; return 0. */
seq_t a(idx_t n)
{
    /* Handle out‑of‑offset requests (not required by OEIS, but safe). */
    if (n == 0) return 0;

    /* Convert n to a type suitable for array indexing (size_t).  n is
       known to be <= 10000 in typical usage, so casting is safe. */
    size_t idx = (size_t)n;

    /* Initial allocation and seed initial values if first call. */
    if (cache == NULL) {
        /* Allocate space for indices 0..6; 0 unused, 1..6 preset. */
        cache_len = 7;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) abort();          /* fatal if allocation fails */

        cache[1] = (seq_t)0;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)130;
        cache[5] = (seq_t)716;
        cache[6] = (seq_t)4018;              /* a(6) from the table */
        max_calculated = 6;
    }

    /* Grow the cache array if a larger n is requested. */
    if (idx > cache_len - 1) {
        size_t new_len = idx + 1;           /* need indices 0..idx */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();          /* fatal on allocation error */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms up to n using the recurrence. */
    if (idx > (size_t)max_calculated) {
        for (size_t k = (size_t)(max_calculated + 1); k <= idx; ++k) {
            if (k <= 6) {
                /* values 1..6 are already seeded; skip computation. */
                continue;
            }
            /* Apply the empirical recurrence. */
            seq_t term = (seq_t)0;
            term += (seq_t)5 * cache[k - 1];
            term += (seq_t)3 * cache[k - 2];
            term += (seq_t)9 * cache[k - 4];
            term += (seq_t)6 * cache[k - 5];
            term += (seq_t)3 * cache[k - 6];
            cache[k] = term;
        }
        max_calculated = n;
    }

    return cache[idx];
}
