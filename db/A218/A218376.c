/*
 * A218376  a(n) = 5^n*sum_{i=1..n} i^5/5^i.
 * https://oeis.org/A218376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(n) = 5*a(n-1) + n^5 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = 0;

    /* One‑time initialization of the cache */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return 0;          /* Should never happen */
        cache[0] = (seq_t)0;
        last = 0;
    }

    /* Grow the cache if necessary */
    if (n > last) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cap   = new_cap;

        /* Compute missing terms up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;

            /* i^5 computed via 128‑bit intermediate to avoid overflow */
            seq_t i2 = (seq_t)((unsigned __int128)i_seq * i_seq);
            seq_t i3 = (seq_t)((unsigned __int128)i2 * i_seq);
            seq_t i5 = (seq_t)((unsigned __int128)i3 * i2);

            seq_t prev = cache[i - 1];
            seq_t val  = (seq_t)((unsigned __int128)prev * 5);
            val += i5;
            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
