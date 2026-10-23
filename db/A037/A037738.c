/*
 * A037738  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3,0.
 * https://oeis.org/A037738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037738:
 *  a(n) is the integer whose base-5 expansion consists of the first n
 *  digits of the infinite periodic sequence 2,1,3,0,2,1,3,0,...
 *  That is,
 *    a(n) = Σ_{k=0}^{n-1} d_k · 5^{n-1-k},   d_k = {2,1,3,0}[k mod 4]
 *
 *  The function caches previously computed terms so that any order of
 *  calls is handled efficiently.  It is O(n) for the first request of
 *  size n and O(1) thereafter.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache array, cache[i] = a(i) */
static idx_t  cache_size = 0; /* number of allocated slots (>= cache_n+1) */
static idx_t  cache_n    = 0; /* highest n for which cache[n] is computed */

static const unsigned digits[4] = {2, 1, 3, 0};

/* Ensure cache has capacity for indices up to need (inclusive).  */
static void ensure_capacity(idx_t need)
{
    if (cache_size > need) return;
    idx_t new_cap = cache_size ? cache_size << 1 : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, the program will crash – acceptable for this
       competitive‑programming context. */
    cache = tmp;
    cache_size = new_cap;
}

/* Return the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, defensive */
    ensure_capacity(n);

    if (n > cache_n) {
        seq_t prev = cache_n ? cache[cache_n] : (seq_t)0;
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            unsigned d = digits[(i - 1) & 3u];  /* i-1 mod 4 */
            prev = prev * (seq_t)5 + (seq_t)d;
            cache[i] = prev;
        }
        cache_n = n;
    }
    return cache[n];
}
