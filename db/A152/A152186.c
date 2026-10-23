/*
 * A152186  a(1)=2; then: if n is even, a(n)=next_square_after a(n - 1), else
 *          a(n)=next_prime_after a(n - 1).
 * https://oeis.org/A152186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1: if n is even, a(n)=next_square_after a(n‑1),
 *            else a(n)=next_prime_after a(n‑1)
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

/* smallest perfect square strictly greater than x */
static seq_t next_square_after(seq_t x)
{
    seq_t s = floor_sqrt(x);          /* s = ⌊√x⌋  */
    if (s * s <= x)                   /* if x itself is a square or less */
        s = s + (seq_t)1;
    return s * s;
}

/* smallest prime strictly greater than x */
static seq_t next_prime_after(seq_t x)
{
    seq_t cand = x + (seq_t)1;
    if (cand <= (seq_t)2)
        return (seq_t)2;              /* 2 is the first prime */
    if ((cand & (seq_t)1) == (seq_t)0) /* make it odd */
        cand = cand + (seq_t)1;
    while (!is_prime_big(cand))
        cand = cand + (seq_t)2;
    return cand;
}

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static idx_t cap      = 0;   /* size of allocation  (≥ last index + 1) */
    static idx_t last     = 0;   /* highest index for which cache is valid */
    static bool   init     = false;

    /* initialization: a(1)=2 */
    if (!init) {
        cap   = 2;                             /* allow indices 0 and 1 */
        cache = (seq_t*)realloc(NULL, cap * sizeof(seq_t));
        if (!cache)
            abort();                           /* insufficient memory */
        cache[0] = (seq_t)0;                   /* unused */
        cache[1] = (seq_t)2;
        last   = 1;
        init   = true;
    }

    /* grow the cache if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                           /* insufficient memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if ((i & (idx_t)1) == (idx_t)0)      /* even i → next square */
            cache[i] = next_square_after(prev);
        else                                 /* odd i  → next prime  */
            cache[i] = next_prime_after(prev);
    }
    last = n;
    return cache[n];
}
