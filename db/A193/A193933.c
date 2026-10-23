/*
 * A193933  E.g.f. A(x) = exp(x+x^2+x^3+x^4+x^5+x^6+x^7).
 * https://oeis.org/A193933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*sum_{k=1}^{min(7,n)} k*(n-1)_{k-1}*a(n-k)/ (n-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        or, equivalently: a(0)=1; for n>=1: 
 *        a(n) = sum_{k=1}^{min(7,n)} k * prod_{t=0}^{k-2}(n-1-t) * a(n-k)  */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache of computed a(i) */
static idx_t  cache_size = 0;        /* number of computed elements */

/* Compute a(n) for all n up to the requested value, using the recurrence above */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return;          /* allocation failure – caller will return 0 */
        cache[0]  = (seq_t)1;        /* a(0) = 1 */
        cache_size = 1;
    }

    if (n < cache_size) return;      /* already computed */

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure – caller will return 0 */
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        seq_t prod = 1;              /* product for current k (k=1 => prod=1) */
        idx_t limit = i < 7 ? i : 7;

        for (idx_t k = 1; k <= limit; ++k) {
            if (k > 1) {
                /* prod = prod * (i - (k-1)); all operands are unsigned */
                prod *= (seq_t)(i - (k - 1));
            }
            /* term = k * prod * cache[i-k] */
            sum += (seq_t)k * prod * cache[i - k];
        }
        cache[i] = sum;
    }
    cache_size = new_size;
}

/* Public function asked for in the problem statement */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_size) return cache[n];
    return (seq_t)0;                  /* on memory allocation failure */
}
