/*
 * A174318  a(n) = ceiling(n!/e) with e = A001113 = exp(1).
 * https://oeis.org/A174318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n!/e) a(2n) = D(2n) (derangement number) a(2n+1)
 *            = D(2n+1) + 1 (add 1 for odd indices)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute a(n) for n >= 0.
   Uses memoized derangement numbers D(k).
   Cache grows as needed, filling all indices up to the current n. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized derangements D(k) */
    static size_t cache_sz = 0;   /* number of valid entries in cache */

    /* Grow cache to accommodate index n, recomputing only new entries. */
    if ((size_t)n + 1 > cache_sz) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory guard */

        cache = tmp;
        /* Fill new entries using the recurrence D(0)=1, D(1)=0 */
        for (size_t k = cache_sz; k <= (size_t)n; ++k) {
            if (k == 0)
                cache[0] = (seq_t)1;            /* D(0) */
            else if (k == 1)
                cache[1] = (seq_t)0;            /* D(1) */
            else {
                seq_t sum = cache[k-1] + cache[k-2];
                cache[k] = ((seq_t)(k-1)) * sum; /* D(k) = (k-1)*(D(k-1)+D(k-2)) */
            }
        }
        cache_sz = new_cap;
    }

    /* Retrieve derangement number for n. */
    seq_t der = cache[(size_t)n];

    /* For odd indices add one. */
    if ((n & (idx_t)1) != 0)
        der += (seq_t)1;

    return der;
}
