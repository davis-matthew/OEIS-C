/*
 * A076248  Trajectory of 1059831 under the Reverse and Add! operation carried
 *          out in base 4, written in base 10.
 * https://oeis.org/A076248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   reverse_base4_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A076248 – trajectory of 1059831 under the Reverse and Add
   operation carried out in base 4, written in base 10.
   Formula (recursive):
     a(0) = 1059831
     a(n) = a(n-1) + reverse_base4(a(n-1))   for n >= 1
   where reverse_base4(x) reads the base‑4 digits of x forwards and
   returns the number represented by those digits read backwards. */
#include <stdlib.h>
#include "../utils/reverse_base4_seq.h"

/* Cache of computed terms.  cache[i] holds a(i).  Cache is grown
   incrementally and reused for subsequent calls. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains at least terms up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;  /* already available */

    /* Allocate sufficient space. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – terminate */ abort(); }
    cache = new_cache;

    /* Initialise a(0) if this is the first allocation. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1059831;
        cache_len = 1;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t rev = reverse_base4_seq(cache[i-1]);
        cache[i] = cache[i-1] + rev;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
