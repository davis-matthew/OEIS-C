/*
 * A239064  Positive integers whose square can be expressed as
 *          k*x^2+(k+1)*x+(k+2) with k and x positive integers.
 * https://oeis.org/A239064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 116, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239064: a(n) is the nth positive integer a for which ∃ positive integers k,x
   with a² = k*x² + (k+1)*x + (k+2).  
   The implementation searches a in ascending order, testing the Diophantine condition by
   iterating over x until the numerator becomes smaller than the denominator.  Cached
   results are stored for repeated calls. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint64_t used in the loop counters */

static seq_t *cache = NULL;       /* cached sequence values                */
static size_t cache_size = 0;     /* number of stored values               */
static size_t cache_cap   = 0;     /* capacity of the cache array           */
static seq_t next_candidate = 1;   /* integer to try next (starts from 2)   */

/* small helper to grow the cache when more space is required */
static void ensure_cache_capacity (size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 16;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = cap;
}

/* test whether a satisfies the Diophantine condition: ∃ x ≥ 1 with
   a² = k*x² + (k+1)*x + (k+2) for some k ≥ 1.  */
static bool is_valid (seq_t a)
{
    seq_t a2 = a * a;                     /* a² pre‑computed once */
    for (uint64_t xi = 1; ; ++xi) {
        seq_t x = (seq_t)xi;
        seq_t denom = x * x + x + 1;      /* x² + x + 1        */
        /* If a² < denom + x + 2 there cannot be k ≥ 1 for larger x,
           so we stop early. */
        seq_t limit = denom + x + 2;
        if (a2 < limit) break;
        seq_t numer = a2 - x - 2;         /* a² – (x+2)        */
        if (numer % denom == 0) return true;
    }
    return false;
}

seq_t a (idx_t n)
{
    /* Ensure that the cache contains at least n terms.        */
    while (cache_size < (size_t)n) {
        ++next_candidate;                /* try next integer a   */
        if (is_valid(next_candidate)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
    }
    return cache[n-1];                    /* zero‑based indexing  */
}
