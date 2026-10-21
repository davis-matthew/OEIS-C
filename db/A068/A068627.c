/*
 * A068627  a(0) = 0, a(n) = a(n-1) + n if n does not divide a(n-1). a(n) =
 *          a(n-1) - n if n divides a(n-1). a(n) = n if a(n-1) = 0.
 * https://oeis.org/A068627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 441, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A068627, defined by:
   a(0) = 0
   a(n) = a(n‑1) + n           if n does not divide a(n‑1)
          a(n‑1) – n           if n divides a(n‑1)
          n                    if a(n‑1) = 0
   (This is written to compute a(n) for any n ≥ 0, heavy‑lifting the
   recursion with memoised storage and working in static memory.)
*/

#include <stdlib.h>

static seq_t *cache      = NULL;   /* array of terms */
static idx_t  cache_cap  = 0;      /* allocated capacity */
static idx_t  last_comp  = 0;      /* highest index already computed */

/* Grow the cache to hold at least indices 0..n inclusive. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;          /* already large enough */

    idx_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;  /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache = tmp;                         /* allocator guarantees malloc‑compatible null */
    cache_cap = new_cap;
}

/* Return a(n) for OEIS sequence A068627.  Handles arbitrary call order. */
seq_t a(idx_t n)
{
    /* Special case for the first term: a(0) = 0 */
    if (n == 0) return (seq_t)0;

    ensure_capacity(n);

    /* Initialise a(0) on first use */
    if (last_comp == 0 && cache[0] == (seq_t)0)
        cache[0] = (seq_t)0;             /* trivial but explicit */

    /* Compute any missing values up to n */
    for (idx_t i = last_comp + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if (prev == (seq_t)0)
            cache[i] = (seq_t)i;                // a(i) = i when a(i-1)=0
        else if (prev % (seq_t)i == 0)
            cache[i] = prev - (seq_t)i;          // subtract when divisible
        else
            cache[i] = prev + (seq_t)i;          // otherwise add
    }

    last_comp = n;
    return cache[n];
}
