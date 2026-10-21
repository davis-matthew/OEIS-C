/*
 * A221023  Equals one maps: number of n X 5 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal, vertical and antidiagonal neighbors in a random 0..3 n
 *          X 5 array.
 * https://oeis.org/A221023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 36*a(n-1) - 128*a(n-2) for n>5
 * Initial values: a(1)=16, a(2)=726, a(3)=30167,
 *                 a(4)=1038208, a(5)=33512960
 */
#include <stdlib.h>

/* memoised recurrence implementation */
static seq_t *cache = NULL;          /* array of terms a(1)...a(max) */
static idx_t cache_len = 0;          /* highest index already computed */
static size_t cache_cap = 0;         /* allocated capacity */

/* ensure the cache can hold up to 'need' terms */
static void grow_cache(idx_t need)
{
    if ((size_t)need <= cache_cap)
        return;
    size_t new_cap = need * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                  /* fatal allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined; return 0 for safety */

    /* compute up to n if necessary */
    if (cache_len < n) {
        grow_cache(n);

        idx_t i;
        for (i = cache_len + 1; i <= n; ++i) {
            /* store 1‑based indices into 0‑based array */
            size_t idx = (size_t)(i - 1);

            if (i == 1)
                cache[idx] = 16;
            else if (i == 2)
                cache[idx] = 726;
            else if (i == 3)
                cache[idx] = 30167;
            else if (i == 4)
                cache[idx] = 1038208;
            else if (i == 5)
                cache[idx] = 33512960;
            else
                cache[idx] = 36 * cache[idx - 1] - 128 * cache[idx - 2];
        }
        cache_len = n;
    }
    return cache[(size_t)(n - 1)];
}
