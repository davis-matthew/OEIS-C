/*
 * A118551  a(0)=1. a(n) = a(n-1)*2, if n is in the sequence. a(n) = a(n-1) +
 *          1 if n is missing from the sequence.
 * https://oeis.org/A118551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence: a(0)=1.  For n>0,
   a(n)=a(n‑1)*2  if n occurs as a term a(k) for some k<n,
   otherwise a(n)=a(n‑1)+1.  The terms are strictly increasing, so
   membership of n can be tested by binary search on the already
   computed terms. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of already computed terms   */
    static size_t cap  = 0;        /* allocated capacity in elements     */
    static size_t size = 0;        /* number of terms stored             */

    /* first call: initialise cache with the first term */
    if (!cache) {
        cap  = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
        size = 1;
    }

    /* convert the index n to size_t for array indexing */
    size_t idx_n = (size_t)n;
    if (idx_n >= SIZE_MAX) return (seq_t)0;  /* index too large for this build */

    /* grow the cache so that we can compute up to index n */
    while (size <= idx_n) {
        /* ensure at least one more slot */
        if (size == cap) {
            size_t new_cap = cap << 1;
            if (new_cap == 0) new_cap = 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        /* membership test: is idx_n present in cache[0 .. size-1]? */
        bool in_seq = false;
        size_t lo = 0, hi = size;
        seq_t target = (seq_t)idx_n;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            seq_t v = cache[mid];
            if (v == target) { in_seq = true; break; }
            if (v < target)  lo = mid + 1;
            else             hi = mid;
        }

        seq_t prev = cache[size - 1];
        cache[size] = in_seq ? (prev * (seq_t)2) : (prev + (seq_t)1);
        size++;
    }

    return cache[idx_n];
}
