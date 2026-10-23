/*
 * A035523  Reverse and add (in base 3).
 * https://oeis.org/A035523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   reverse_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035523: Reverse and add (base 3)
 *  Recurrence:  a(0) = 1,  a(n) = a(n-1) + reverse_base3(a(n-1))
 *  where reverse_base3(x) returns the integer obtained by reversing the
 *  base‑3 digits of x.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_base3.h"

/* Reverse the base‑3 digits of x and return the value. */

/* Cached array of computed terms. */
static seq_t *seq_cache   = NULL;   /* array of a(k), k = 0 .. last_computed */
static idx_t  cache_size  = 0;      /* number of allocated slots  */
static idx_t  last_computed = 0;    /* last index for which value is known */

/* Ensure the cache can hold indices up to n. */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* in case of allocation failure */
        seq_cache = new_cache;
        cache_size = new_size;
    }
}

/* Starting at the next uncomputed index, fill the cache up to n. */
static void fill_cache(idx_t n)
{
    /* Initialisation on first call. */
    if (seq_cache == NULL) {
        ensure_cache(0);
        seq_cache[0] = 1;                 /* a(0) = 1 */
        last_computed = 0;
    }

    if (n == 0) return;                   /* a(0) already set */

    if (last_computed < n) {
        ensure_cache(n);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t prev = seq_cache[i - 1];
            seq_t rev  = reverse_base3(prev);
            seq_cache[i] = prev + rev;
        }
        last_computed = n;
    }
}

seq_t a(idx_t n)
{
    fill_cache(n);
    return seq_cache[n];
}
