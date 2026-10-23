/*
 * A198628  a(n) = 1-2^n+3^n-4^n+5^n.
 * https://oeis.org/A198628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=1..5} (-1)^{j+1} j^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_i = (idx_t)-1;

    if (!cache) {
        /* initialise with first five terms */
        cap = 5;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;   /* 1 - 2^0 + 3^0 - 4^0 + 5^0 = 1  */
        cache[1] = (seq_t)3;   /* 1 - 2^1 + 3^1 - 4^1 + 5^1 = 3  */
        cache[2] = (seq_t)15;  /* ...                         */
        cache[3] = (seq_t)81;
        cache[4] = (seq_t)435;
        max_i = 4;
    }

    if (n <= max_i) return cache[n];

    /* grow the cache so that it can hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        while ((size_t)n + 1 > newcap) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return cache[max_i];
        cache = tmp;
        cap = newcap;
    }

    /* compute using the linear recurrence:
       a(k) = 15 a(k‑1) – 85 a(k‑2) + 225 a(k‑3) – 274 a(k‑4) + 120 a(k‑5)
       for k ≥ 5                                                   */
    for (idx_t i = max_i + 1; i <= n; ++i) {
        seq_t t0 = cache[i-1];
        seq_t t1 = cache[i-2];
        seq_t t2 = cache[i-3];
        seq_t t3 = cache[i-4];
        seq_t t4 = cache[i-5];

        seq_t val = t0 * (seq_t)15;
        val = val - t1 * (seq_t)85;
        val = val + t2 * (seq_t)225;
        val = val - t3 * (seq_t)274;
        val = val + t4 * (seq_t)120;
        cache[i] = val;
    }

    max_i = n;
    return cache[n];
}
