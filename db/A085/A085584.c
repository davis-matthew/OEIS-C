/*
 * A085584  Number of (3412,2341)-, (3412,4123)- and (3412,52341)-avoiding
 *          involutions in S_n.
 * https://oeis.org/A085584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=4, a(4)=9,
   a(n)=3*a(n-1)-a(n-2)-3*a(n-3)+3*a(n-4) for n≥5. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[n] holds a(n) (n≥1) */
static idx_t cache_cap = 0;      /* allocated size (≥ n+1) */
static idx_t cache_size = 0;     /* highest n computed */

/* Allocate or grow the cache to hold indices up to 'n'. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t newcap = cache_cap ? cache_cap : 8;
    while (newcap <= n)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */
    cache = tmp;
    cache_cap = newcap;
}

/* Compute all terms up to and including n, using the recurrence. */
static void compute_up_to(idx_t n)
{
    if (cache_size == 0) {        /* initialize first four terms */
        ensure_capacity(4);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)9;
        cache_size = 4;
    }
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Recurrence valid for i >= 5 */
        cache[i] = 3 * cache[i-1] - cache[i-2] - 3 * cache[i-3] + 3 * cache[i-4];
    }
    if (n > cache_size)
        cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n<1, return 0 */
    ensure_capacity(n);
    if (cache_size < n)
        compute_up_to(n);
    return cache[n];
}
