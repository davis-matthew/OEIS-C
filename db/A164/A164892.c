/*
 * A164892  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(0) = 1, a(1) = 18, …,
 *            a(7) = 434476242 a(8) = 16*(a(7)+…+a(1)) – 136*a(0) + 1 a(n) =
 *            16*(a(n-1)+…+a(n-7)) – 136*a(n-8) for n ≥ 9 This function
 *            memoises previously computed values
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* memoisation cache */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_end = 0;   /* first index not computed yet */

/* known initial values a(0) … a(7) */
static const seq_t init_vals[8] = {
    (seq_t)1ULL,
    (seq_t)18ULL,
    (seq_t)306ULL,
    (seq_t)5202ULL,
    (seq_t)88434ULL,
    (seq_t)1503378ULL,
    (seq_t)25557426ULL,
    (seq_t)434476242ULL
};

/* expand cache to at least 'needed' capacity */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();              /* allocation failure */
    /* new slots are zeroed by realloc when new_cap > old_cap */
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute all values up to and including index n */
static void compute_till(idx_t n)
{
    if (n + 1 <= cache_end) return;          /* already computed */

    ensure_capacity(n + 1);

    /* initialise missing initial terms */
    if (cache_end < 8) {
        idx_t lim = (n + 1 < 8) ? (n + 1) : 8;
        for (idx_t i = cache_end; i < lim; ++i)
            cache[i] = init_vals[i];
    }

    /* generate remaining terms */
    for (idx_t i = cache_end; i <= n; ++i) {
        if (i < 8) continue;                 /* already set */

        /* compute sum of previous 7 terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 7; ++k)
            sum += cache[i - k];

        if (i == 8) {
            /* special case: a(8) = 16*sum - 136*a0 + 1 */
            cache[i] = sum * (seq_t)16
                       - cache[0] * (seq_t)136
                       + (seq_t)1;
        } else {
            /* standard recurrence */
            cache[i] = sum * (seq_t)16
                       - cache[i - 8] * (seq_t)136;
        }
    }

    cache_end = n + 1;
}

/* public accessor */
seq_t a(idx_t n)
{
    compute_till(n);
    return cache[n];
}
