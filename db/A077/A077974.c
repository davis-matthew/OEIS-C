/*
 * A077974  Expansion of 1/(1+x+2*x^3).
 * https://oeis.org/A077974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A077974: Expansion of 1/(1+x+2*x³)
   Formula: a(0)=1, a(1)=-1, a(2)=1,
            a(n) = -a(n-1) - 2*a(n-3)   for n ≥ 3
   Generates the integer sequence 1, -1, 1, -3, 5, -7, 13, -23, …
   The implementation memoises results in a dynamic array so that
   calls may be out‑of‑order.  All arithmetic is performed in type seq_t
   to respect the required integer precision.  No global state other
   than the cache is used, and the routine is O(n) in the largest
   argument supplied. */
#include <stdlib.h>

/* cache for already computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure terms up to index n are computed. */
static void ensure_terms(idx_t n)
{
    if (n + 1 <= cache_size)           /* already available */
        return;

    idx_t new_size = n + 1;

    /* reallocate (if necessary) */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)      /* allocation failure – leave cache unchanged */
        return;
    cache = tmp;

    /* fill missing entries */
    for (idx_t i = cache_size; i < new_size; ++i) {
        switch (i) {
        case 0:  cache[i] = (seq_t)1;      break;
        case 1:  cache[i] = (seq_t)-1;     break;
        case 2:  cache[i] = (seq_t)1;      break;
        default: cache[i] = -cache[i-1] - ((seq_t)2 * cache[i-3]);
                 break;
        }
    }
    cache_size = new_size;
}

/* public interface: a(n) */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n];
}
