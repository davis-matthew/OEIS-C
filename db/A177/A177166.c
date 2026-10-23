/*
 * A177166  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=9, k=0 and l=-1.
 * https://oeis.org/A177166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=9, and for n>=1, a(n+1)=sum_{p=0..n} a(p)*a(n-p) -
 *            1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* sequence values       */
static idx_t  cache_len = 0;          /* allocated number of slots */
static idx_t  computed_len = 0;       /* values computed up to computed_len-1 */

static void ensure_size(idx_t need)
{
    if (cache_len >= need)
        return;
    idx_t new_len = cache_len ? cache_len << 1 : 1;
    while (new_len < need)
        new_len <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory */
    cache = tmp;
    /* zero newly allocated slots (safe for unsigned and trivial types) */
    for (idx_t i = cache_len; i < new_len; ++i)
        cache[i] = 0;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Make sure array can hold a[n] */
    ensure_size(n + 1);

    /* Initialise first two terms once */
    if (computed_len == 0) {
        if (cache_len < 2) ensure_size(2);
        cache[0] = 1;
        cache[1] = 9;
        computed_len = 2;
    }

    /* Return if already computed */
    if (n < computed_len)
        return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t m = computed_len; m <= n; ++m) {
        seq_t sum = 0;
        /* For m>=2, use recurrence a(m) = sum_{p=0}^{m-1} a(p)*a(m-1-p) - 1 */
        for (idx_t p = 0; p <= m - 1; ++p)
            sum += cache[p] * cache[m - 1 - p];
        cache[m] = sum - (seq_t)1;
    }
    computed_len = n + 1;
    return cache[n];
}
