/*
 * A097596  Expansion of g.f.
 *          x*(1-x+x^5+x^6-x^7+x^9)/(1-2*x+x^4+x^6-2*x^7+x^10).
 * https://oeis.org/A097596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑1) – a(n‑4) – a(n‑6) + 2*a(n‑7) – a(n‑10) * Initial
 *            values: a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=7, a(6)=14, *
 *            a(7)=26, a(8)=48, a(9)=89, a(10)=165
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 74, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for previously computed terms */
static seq_t *a_cache = NULL;     /* array indexed from 1 (a_cache[0] unused) */
static idx_t cache_cap   = 0;     /* currently allocated capacity (last index) */
static idx_t cache_last  = 0;     /* highest index already computed */

/* Ensure the cache can hold terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    /* simple growth: double or just enough */
    idx_t new_cap = (cache_cap > 0) ? (cache_cap << 1) : 16;
    if (new_cap < n)
        new_cap = n;
    void *tmp = realloc(a_cache, (size_t)(new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();              /* out of memory */
    a_cache = (seq_t *)tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1; behaviour for n == 0 is undefined */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset behaviour is irrelevant */

    /* Initialise cache on first call */
    if (a_cache == NULL) {
        ensure_cache(10);       /* capacity for first 10 terms */
        /* a_cache[0] unused */
        a_cache[0] = 0;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)1;
        a_cache[3] = (seq_t)2;
        a_cache[4] = (seq_t)4;
        a_cache[5] = (seq_t)7;
        a_cache[6] = (seq_t)14;
        a_cache[7] = (seq_t)26;
        a_cache[8] = (seq_t)48;
        a_cache[9] = (seq_t)89;
        a_cache[10] = (seq_t)165;
        cache_last = 10;
    }

    if (n <= cache_last)
        return a_cache[n];

    /* Compute missing terms up to n */
    ensure_cache(n);
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t term = a_cache[i - 1];          /* 2*a(i-1) */
        term += a_cache[i - 1];
        term += a_cache[i - 7];              /* +2*a(i-7) */
        term += a_cache[i - 7];
        term -= a_cache[i - 4];
        term -= a_cache[i - 6];
        term -= a_cache[i - 10];
        a_cache[i] = term;
    }
    cache_last = n;
    return a_cache[n];
}
