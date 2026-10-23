/*
 * A170581  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤46: a(n)=2+42*Σ_{k=0}^{n-1}a(k); for n=47:
 *            a(47)=1+42*Σ_{k=0}^{46}a(k)-903*a(0); for n≥48:
 *            a(n)=42*Σ_{k=n-47}^{n-1}a(k)-903*a(n-47);
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;      /* dynamically grown array of a(n) values */
static idx_t cache_size  = 0;          /* number of terms already computed */
static size_t cache_cap  = 0;          /* allocated capacity of cache     */

/* value of cumulative sum Σ_{k=0}^{i} a(k) for the last computed index i */
static seq_t cum_sum     = 0;
/* sum of the last 47 computed terms: Σ_{k=i-47}^{i-1} a(k) when i ≥ 48 */
static seq_t last47_sum  = 0;

/* ensure that cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)   /* already have all needed terms */
        return;

    /* grow the cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term if cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cum_sum  = cache[0];
        cache_size = 1;
    }

    /* compute terms starting from the first missing index */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = 0;

        if (i <= 46) {
            /* a(i) = 2 + 42 * Σ_{k=0}^{i-1} a(k)   (cum_sum holds Σ_{k=0}^{i-1}a(k)) */
            term = (seq_t)2 + (seq_t)42 * cum_sum;
        } else if (i == 47) {
            /* a(47) = 1 + 42*Σ_{k=0}^{46}a(k) - 903*a(0) */
            term = (seq_t)1 + (seq_t)42 * cum_sum - (seq_t)903 * cache[0];
            /* after this term, set last47_sum for the next step */
            last47_sum = cum_sum + term - cache[0];   /* Σ_{k=1}^{47}a(k) */
        } else { /* i >= 48 */
            /* a(i) = 42*Σ_{k=i-47}^{i-1}a(k) - 903*a(i-47) */
            term = (seq_t)42 * last47_sum - (seq_t)903 * cache[i - 47];
            /* slide the window: add term, remove a(i-47) */
            last47_sum += term - cache[i - 47];
        }

        cache[i] = term;
        cum_sum += term;
    }

    cache_size = n + 1;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1)   /* undefined, but prevents underflow on unsigned types */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
