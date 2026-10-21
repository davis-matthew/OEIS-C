/*
 * A056026  Numbers k such that k^14 == 1 (mod 15^2).
 * https://oeis.org/A056026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056026:  Numbers k such that k^14 ≡ 1 (mod 225)
 * Formula:  a(n) = a(n-1) + a(n-4) – a(n-5),
 * initial values: a(1)=1, a(2)=26, a(3)=199, a(4)=224, a(5)=226
 * ----------------------------------------------
 * This function is memoised and works for arbitrary call order.
 */
#include <stdlib.h>

static seq_t *cache = NULL;        /* array storing computed terms   */
static idx_t  cache_len = 0;       /* number of terms currently stored */
static idx_t  cache_cap = 0;       /* allocated capacity (in terms)   */

/* Ensure the cache has space for at least 'need' terms.       */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;

    /* Grow the capacity to at least 'need', doubling each time.   */
    if (cache_cap < 8)
        cache_cap = 8;
    while (cache_cap < need)
        cache_cap <<= 1;

    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Initialise the cache with the five base terms.                */
static void init_cache(void)
{
    ensure_cache(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)26;
    cache[2] = (seq_t)199;
    cache[3] = (seq_t)224;
    cache[4] = (seq_t)226;
    cache_len = 5;
}

/*  The required sequence function                                       */
seq_t a(idx_t n)
{
    if (n == 0) {  /* no defined value for n=0, but we return 0 to avoid UB */
        return (seq_t)0;
    }

    if (cache_len == 0)
        init_cache();

    if (n > cache_len) {
        /* Extend the cache up to n using the recurrence                 */
        ensure_cache(n);
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* a(k) = a(k-1) + a(k-4) – a(k-5)                      */
            seq_t val = cache[k - 2]          /* a(k-1)     */
                      + cache[k - 5]          /* a(k-4)     */
                      - cache[k - 6];         /* a(k-5)     */
            cache[k - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];   /* 1‑based indexing */
}
