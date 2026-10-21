/*
 * A080253  a(n) is the number of elements in the Coxeter complex of type B_n
 *          (or C_n).
 * https://oeis.org/A080253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 * a(n) = 1 + Σ_{r=1..n} 2^r * binomial(n,r) * a(n-r) (n
 *            ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for computed terms (index = n) */
static seq_t *cache_arr = NULL;
static size_t alloc_size = 0;       /* size of allocated memory */
static idx_t  computed_size = 0;    /* number of terms actually computed */

/* expand cache_arr so that it can hold at least n elements (index 0..n-1) */
static void ensure_cache(idx_t n_needed)
{
    if (alloc_size >= (size_t)n_needed)
        return;
    size_t new_size = (alloc_size > (size_t)n_needed / 2) ?
                      alloc_size * 2 : (size_t)n_needed;
    seq_t *new_arr = (seq_t *)realloc(cache_arr, new_size * sizeof(seq_t));
    if (!new_arr)
        abort();                      /* memory allocation failure */
    cache_arr = new_arr;
    alloc_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) {                     /* base case */
        if (computed_size == 0) {
            ensure_cache(1);
            cache_arr[0] = (seq_t)1;
            computed_size = 1;
        }
        return cache_arr[0];
    }

    if (n < computed_size)            /* already computed */
        return cache_arr[n];

    /* compute all terms from computed_size up to n */
    idx_t start = computed_size;      /* first index to compute */
    ensure_cache(n + 1);

    for (idx_t idx = start; idx <= n; ++idx) {
        seq_t value = (seq_t)1;        /* the leading constant term */
        seq_t pow2r = (seq_t)2;        /* 2^1 for r=1 */
        seq_t comb = (seq_t)idx;       /* C(idx,1) */

        for (idx_t r = 1; r <= idx; ++r) {
            seq_t term = pow2r * comb * cache_arr[idx - r];
            value += term;

            if (r < idx) {            /* prepare C(idx,r+1) and 2^{r+1} */
                comb = comb * (seq_t)(idx - r) / (seq_t)(r + 1);
                pow2r <<= 1;           /* multiply by 2 */
            }
        }
        cache_arr[idx] = value;
    }
    computed_size = n + 1;            /* now at least n is computed */
    return cache_arr[n];
}
