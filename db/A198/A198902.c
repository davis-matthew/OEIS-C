/*
 * A198902  Number of nX4 0..4 arrays with values 0..4 introduced in row major
 *          order and no element equal to any horizontal or vertical neighbor.
 * https://oeis.org/A198902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 163*a(n-1) - 3751*a(n-2) + 32097*a(n-3) - 120287*a(n-4) +
 *            201471*a(n-5) - 148296*a(n-6) + 38604*a(n-7) Initial values:
 *            a(1)=5 , a(2)=380 , a(3)=50384 , a(4)=6907736 , a(5)=948656912 ,
 *            a(6)=130292546801 , a(7)=17895005957823
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* initial terms (1‑based) */
static const seq_t init_vals[8] = {
    /* 0th index unused */ (seq_t)0,
    (seq_t)5,
    (seq_t)380,
    (seq_t)50384,
    (seq_t)6907736,
    (seq_t)948656912,
    (seq_t)130292546801,
    (seq_t)17895005957823
};

/* cache of computed terms (1‑based) */
static seq_t *cache = NULL;          /* dynamic array of seq_t */
static idx_t  cache_len = 0;         /* number of terms stored (>=7 after init) */

/* initialise the cache with the first 7 terms */
static void init_cache(void)
{
    cache_len = 7;
    cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
    if (!cache) abort();          /* allocation failure – terminate */
    for (idx_t i = 1; i <= cache_len; ++i)
        cache[i] = init_vals[i];
}

/* sequence function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for non‑positive n */

    /* initialise cache if needed */
    if (!cache)
        init_cache();

    /* return a cached value if already computed */
    if (n <= cache_len)
        return cache[n];

    /* compute terms up to n, extending the cache as required */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        pos += (seq_t)163    * cache[k - 1];
        neg += (seq_t)3751   * cache[k - 2];
        pos += (seq_t)32097  * cache[k - 3];
        neg += (seq_t)120287 * cache[k - 4];
        pos += (seq_t)201471 * cache[k - 5];
        neg += (seq_t)148296 * cache[k - 6];
        pos += (seq_t)38604  * cache[k - 7];

        seq_t val = pos - neg;   /* final term for index k */

        /* extend the cache array */
        if (cache_len < k)
        {
            cache = (seq_t *)realloc(cache, (k + 1) * sizeof(seq_t));
            if (!cache) abort(); /* allocation failure – terminate */
            cache_len = k;
        }
        cache[k] = val;
    }

    return cache[n];
}
