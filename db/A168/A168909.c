/*
 * A168909  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, let p(n)=1 if n==21, 2 if 1≤n≤20, 0 otherwise.
 *            Then a(n)=p(n)+38·(a(n-1)+a(n-2)+…+a(n-20))−741·a(n-21) with
 *            a(k)=0 for k<0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* extern types are supplied by the build system */
static seq_t *cache = NULL;     /* computed sequence values */
static size_t cache_cap = 0;    /* allocated size of cache */
static idx_t  cache_len = 0;    /* number of values stored (next free index) */

/* Ensure the cache can hold values up to index n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* Resize if necessary. */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute sequence terms from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Sum of the previous up to 20 terms. */
        seq_t sum20 = (seq_t)0;
        for (idx_t j = 1; j <= 20 && i >= j; ++j)
            sum20 += cache[i - j];

        /* Term involving a(i-21). */
        seq_t term21 = (seq_t)0;
        if (i >= 21)
            term21 = ((seq_t)741) * cache[i - 21];

        /* Coefficient from the numerator polynomial. */
        seq_t p = (seq_t)0;
        if (i == 0 || i == 21)
            p = (seq_t)1;
        else if (i <= 20)
            p = (seq_t)2;

        /* Apply the recurrence. */
        cache[i] = p + ((seq_t)38) * sum20 - term21;
    }

    cache_len = n + 1;
}

/* Entry point: return the n-th term of A168909. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
