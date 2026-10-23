/*
 * A015091  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=7.
 * https://oeis.org/A015091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{n-1} 7^i * a(i) * a(n-1-i) a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Resize an array of seq_t up to 'need' elements.  Caller updates
   *cap when the function returns. */
static void ensure_capacity(size_t *cap, seq_t **arr, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;     /* allocated capacity of seq_cache */
static idx_t  cache_max = (idx_t)-1;   /* highest index computed */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure we have space for indices up to n. */
    ensure_capacity(&seq_cap, &seq_cache, (size_t)(n + 1));

    /* Initialise a(0) on first use. */
    if (seq_cache[0] == 0) seq_cache[0] = 1;
    if (cache_max == (idx_t)-1) cache_max = 0;

    /* Return cached value if already computed. */
    if (cache_max >= n) return seq_cache[n];

    /* Compute missing terms up to n. */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t pow = 1;      /* 7^i, i starts at 0 */

        for (idx_t i = 0; i < m; ++i) {
            idx_t j = m - 1 - i;
            sum += pow * seq_cache[i] * seq_cache[j];
            pow *= (seq_t)7;  /* next power of 7 */
        }
        seq_cache[m] = sum;
    }
    cache_max = n;
    return seq_cache[n];
}
