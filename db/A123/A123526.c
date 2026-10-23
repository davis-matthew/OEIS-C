/*
 * A123526  Octanacci numbers.
 * https://oeis.org/A123526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=1 for 1 ≤ n ≤ 8; a(n)=a(n-1)+…+a(n-8) for n>8
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 70, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n‑th Octanacci number.
 * seq_t is the sequence type (unsigned 8‑, 16‑, 32‑, 64‑, 128‑bit or arbitrary precision).
 * idx_t is the index type (uint64_t). */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n<1, return 0 to be safe */
        return (seq_t)0;

    /* static cache: cache[i] = a(i), 1 ≤ i ≤ last */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size (indices 0 … cap‑1) */
    static size_t last  = 0;   /* largest computed index (≥0) */

    /* Ensure cache is large enough to hold a(n) */
    if (n + 1 > cap) {
        /* grow to at least n+1, double for amortized O(n) growth */
        size_t new_cap = ((n + 1) > cap * 2) ? (n + 1) : (cap * 2 ? cap * 2 : 16);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – in a real program you might abort here */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize first eight values if not yet done */
    if (last < 8) {
        size_t init_to = (cap > 8) ? 8 : (cap - 1);
        for (size_t i = 1; i <= init_to; ++i)
            cache[i] = (seq_t)1;
        last = init_to;
    }

    /* Handle small n directly */
    if ((idx_t)last >= n)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (int j = 1; j <= 8; ++j)
            sum += cache[i - j];
        cache[i] = sum;
    }
    last = (size_t)n;

    return cache[(size_t)n];
}
