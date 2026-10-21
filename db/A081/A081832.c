/*
 * A081832  a(1)=a(2)=1, a(n) = a(n+1-2*a(n-1)) + a(n-2*a(n-2)).
 * https://oeis.org/A081832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1, a(n) = a(n+1-2*a(n-1)) + a(n-2*a(n-2)) (n ≥ 3)
 * Range:     u8 n <= 1003, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------- memory handling ------------- */
static seq_t *cache      = NULL; /* array of previously computed terms   */
static idx_t cache_cap   = 0;    /* current capacity of the array        */
static idx_t cache_len   = 0;    /* number of terms stored (≥ 0)        */

static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)            /* already large enough          */
        return;

    /* next power of two ≥ need; grow by factor of two   */
    idx_t new_cap = cache_cap ? cache_cap << 1 : 2;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_ptr;
#ifdef __cplusplus
    /* allocate fresh array and copy existing terms     */
    new_ptr = new seq_t[new_cap];
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i)
            new_ptr[i] = cache[i];
        delete [] cache;
    }
#else
    /* reallocate raw storage (seq_t is an unsigned integer type) */
    new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    if (!new_ptr)
        abort();                      /* unrecoverable, terminate   */

    cache      = new_ptr;
    cache_cap  = new_cap;
}

/* ------------- sequence computation ------------- */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined, but return 0 */
        return (seq_t)0;

    /* initialize base values if not yet present         */
    if (cache_len == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)1;          /* a(2) */
        cache_len = 2;
    }

    /* answer is already known */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend computation up to n                      */
    ensure_capacity(n);
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t prev1 = cache[k - 2];   /* a(k-1) */
        seq_t prev2 = cache[k - 3];   /* a(k-2) */

        /* compute the two indices used in the recurrence */
        seq_t idx1_val = (seq_t)k + 1 - (seq_t)2 * prev1;
        seq_t idx2_val = (seq_t)k - (seq_t)2 * prev2;

        idx_t idx1 = (idx_t)idx1_val;   /* guaranteed < k */
        idx_t idx2 = (idx_t)idx2_val;   /* guaranteed < k */

        cache[k - 1] = cache[idx1 - 1] + cache[idx2 - 1];
    }

    cache_len = n;
    return cache[n - 1];
}
