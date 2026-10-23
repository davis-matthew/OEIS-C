/*
 * A169431  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) -
 *            32*(a(n-1)+…+a(n-31)) + 528*a(n-32) = p(n), * where p(0)=1,
 *            p(1..31)=2, p(32)=1 and p(n)=0 for n>32. * For n ≥ 33 the
 *            homogeneous recurrence is * a(n) = 32*(a(n-1)+…+a(n-31)) -
 *            528*a(n-32)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Small helper that returns the non‑homogeneous term p(n). */
static inline seq_t p_val(idx_t n)
{
    if (n == 0 || n == 32)
        return 1;
    if (n > 0 && n < 32)
        return 2;
    return 0;
}

/* Compute a(n) for any n ≥ 0.  The sequence values are memoised
 * in a static cache so that subsequent calls are O(1) for already
 * computed indices. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;      /* array of a(k) for k < cache_size. */
    static idx_t cache_size = 0;     /* number of elements currently filled. */
    static idx_t cache_cap   = 0;    /* capacity of the cache array. */

    /* Ensure the cache is large enough to hold up to index n. */
    while (cache_cap <= n) {
        idx_t new_cap = (cache_cap == 0) ? (n + 1) : (cache_cap << 1);
        /* realloc returns NULL on failure; abort on error. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    while (cache_size <= n) {
        seq_t sum = 0;

        /* Sum the previous up to 31 terms (indices < 0 are treated as 0). */
        for (idx_t i = 1; i <= 31 && i <= cache_size; ++i)
            sum += cache[cache_size - i];

        seq_t term = 32 * sum;
        if (cache_size >= 32)
            term -= 528 * cache[cache_size - 32];

        term += p_val(cache_size);
        cache[cache_size] = term;
        ++cache_size;
    }

    return cache[n];
}
