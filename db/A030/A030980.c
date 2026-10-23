/*
 * A030980  Number of planted noncrossing bushes with n nodes; i.e., rooted
 *          noncrossing trees with n nodes, root degree 1 and no nonroot nodes
 *          of degree 1.
 * https://oeis.org/A030980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: -2*n*(2*n-1)*a(n) +3*n*(n-2)*a(n-1) +30*(2*n-3)*(n-2)*a(n-2)
 *               +76*(n-2)*(n-3)*a(n-3)=0
 * Initial values: a(1)=1, a(2)=0, a(3)=3
 * This implementation memoises computed values and uses the
 * recurrence for n≥4.  For seq_t types that fit in 64 bits
 * 128‑bit intermediate arithmetic is used to avoid overflow.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t for idx_t */

static seq_t *cache = NULL;         /* array of computed values (0‑based) */
static idx_t  cache_alloc = 0;      /* allocated size (number of slots)      */
static idx_t  cache_computed = 0;   /* highest index n that has been computed (1‑based) */

/* Ensure the cache has at least 'needed' slots. */
static void
ensure_cache(idx_t needed)
{
    if (needed <= cache_alloc)
        return;
    size_t new_size = (size_t)needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_alloc = needed;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥1 by the specification. */
    if (n == 0) return 0;   /* defensive, should never happen */

    ensure_cache(n);
    if (n <= cache_computed)
        return cache[n - 1];

    /* Compute from the next missing term up to n. */
    for (idx_t k = cache_computed + 1; k <= n; ++k) {
        if (k == 1) {
            cache[0] = (seq_t)1;
        } else if (k == 2) {
            cache[1] = (seq_t)0;
        } else if (k == 3) {
            cache[2] = (seq_t)3;
        } else {
            /* Use recurrence for k ≥ 4. */
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                /* Use 128‑bit intermediates for small types. */
                unsigned __int128 num1 = (unsigned __int128)3 * k * (k - 2);
                num1 *= (unsigned __int128)cache[k - 2];
                unsigned __int128 num2 = (unsigned __int128)30 * (2 * k - 3) * (k - 2);
                num2 *= (unsigned __int128)cache[k - 3];
                unsigned __int128 num3 = (unsigned __int128)76 * (k - 2) * (k - 3);
                num3 *= (unsigned __int128)cache[k - 4];
                unsigned __int128 numer = num1 + num2 + num3;
                unsigned __int128 denom = (unsigned __int128)2 * k * (2 * k - 1);
                cache[k - 1] = (seq_t)(numer / denom);
            } else {
                /* Arbitrary‑precision path. */
                seq_t num1 = (seq_t)3 * (seq_t)k * (seq_t)(k - 2);
                num1 *= cache[k - 2];
                seq_t num2 = (seq_t)30 * (seq_t)(2 * k - 3) * (seq_t)(k - 2);
                num2 *= cache[k - 3];
                seq_t num3 = (seq_t)76 * (seq_t)(k - 2) * (seq_t)(k - 3);
                num3 *= cache[k - 4];
                seq_t numer = num1 + num2 + num3;
                seq_t denom = (seq_t)2 * (seq_t)k * (seq_t)(2 * k - 1);
                cache[k - 1] = numer / denom;
            }
        }
    }

    cache_computed = n;
    return cache[n - 1];
}
