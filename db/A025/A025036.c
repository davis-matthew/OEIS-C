/*
 * A025036  Number of partitions of { 1, 2, ..., 4n } into sets of size 4.
 * https://oeis.org/A025036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n)! / (n! * (4!)^n) Recurrence: a(0) = 1, a(n) =
 *            (4*n-3)*(2*n-1)*(4*n-1)*a(n-1)/3 for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cache_sz = 0;

    /* Ensure cache[0] = 1 exists */
    if (cache_sz == 0) {
        cache_sz = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();          /* out‑of‑memory */
        cache[0] = (seq_t)1;
    }

    if (n < cache_sz)
        return cache[n];

    /* Expand array to hold up to n */
    if (n + 1 > cache_sz) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!cache) abort();          /* out‑of‑memory */
        for (idx_t i = cache_sz; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t prev = cache[i - 1];
            seq_t term = prev;
            term = term * (seq_t)(4 * i - 3);
            term = term * (seq_t)(2 * i - 1);
            term = term * (seq_t)(4 * i - 1);
            term = term / (seq_t)3;
            cache[i] = term;
        }
        cache_sz = n + 1;
    }

    return cache[n];
}
