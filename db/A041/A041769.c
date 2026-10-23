/*
 * A041769  Denominators of continued fraction convergents to sqrt(405).
 * https://oeis.org/A041769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 322*a(n-2) - a(n-4)  for n>3
   Initial values: a(0)=1, a(1)=8, a(2)=321, a(3)=2576 */

#include <stdlib.h>

/* Cache of computed terms.  Dynamic resizing ensures we always have
   enough space for the current request and the 4 initial values. */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;     /* current allocated size in elements */
static idx_t  computed = 0;      /* highest index that has been computed */

/* Resize the cache array so that it has at least 'needed' elements.
   The array is grown geometrically to keep amortised O(1) cost per
   allocation.  We also ensure it is large enough to hold the four
   initial values, even when the first request is for a very small n. */
static void grow_cache(idx_t needed)
{
    /* The sequence has 4 known initial values.  Guarantee that the
       cache can hold at least those. */
    if (needed < 4)
        needed = 4;

    if (needed <= cache_len)
        return;                    /* already large enough */

    /* Double the size until it is at least 'needed' */
    size_t new_len = cache_len ? cache_len : 1;
    while (new_len < (size_t)needed)
        new_len <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failure: terminate program */
    cache = tmp;
    cache_len = new_len;

    /* Initialise the four base cases only once. */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)321;
        cache[3] = (seq_t)2576;
        computed = 3;           /* last initial index filled */
    }
}

/* Return the n‑th term of A041769.  The function is fully memoised
   and works for arbitrary calling order of n. */
seq_t a(idx_t n)
{
    grow_cache(n + 1);            /* ensure cache[n] exists */

    if (n <= computed)
        return cache[n];

    /* Compute missing terms by the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)322 * cache[i - 2] - cache[i - 4];
        cache[i] = term;
    }
    computed = n;
    return cache[n];
}
