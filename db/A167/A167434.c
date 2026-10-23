/*
 * A167434  Diagonal sums of the Riordan array (1-4*x+4*x^2, x*(1-2*x))
 *          (A167431).
 * https://oeis.org/A167434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 23, i32 n <= 49, i64 n <= 102, i128 n <= 208,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167434 – Diagonal sums of Riordan array (1‑4*x+4*x², x*(1‑2*x))
   G.f.: (1‑2*x)² / (1‑x²+2*x³)
   Recurrence:
        a(0) = 1
        a(1) = –4
        a(2) = 5
        a(n) = a(n‑2) – 2*a(n‑3)   for n ≥ 3
   The recurrence follows from (1‑x²+2*x³)*A(x) = (1‑2*x)².  */
#include <stdlib.h>

/* Memoised storage of previously computed terms. */
static seq_t *cache      = NULL;   /* Array holding a(k).  */
static size_t cache_cap  = 0;      /* Allocated length of cache. */
static idx_t  next_need  = 0;      /* First index not yet populated. */

/* Ensure that cache[n] has been computed and stored.  */
static void
ensure_cache(idx_t n)
{
    /* Expand the array to hold indices 0 .. n.  */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* Allocation failure – abort.  */
            exit(EXIT_FAILURE);
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Initialise the first three terms on the very first call.  */
    if (next_need == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-4;
        cache[2] = (seq_t)5;
        next_need = 3;                 /* indices 0,1,2 are now stored. */
    }

    /* If the requested index is already computed, nothing to do.  */
    if (n < next_need) return;

    /* Compute new terms up to n using the recurrence.  */
    for (idx_t i = next_need; i <= n; ++i) {
        /* a(i) = a(i‑2) – 2*a(i‑3)  for i ≥ 3.
           For i = 0,1,2 the loop does not run because next_need ≥ 3.  */
        cache[i] = cache[i - 2] - (cache[i - 3] * ((seq_t)2));
    }
    next_need = n + 1;   /* All indices up to n are now filled. */
}

/* Public function returning a(n).  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
