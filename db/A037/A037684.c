/*
 * A037684  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3,2.
 * https://oeis.org/A037684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-4) - 7*a(n-5) Initial values: a(1)=1,
 *            a(2)=7, a(3)=52, a(4)=366, a(5)=2563
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#if defined(__cplusplus)
#include <new>
#endif

/* static cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_size   = 0;   /* number of terms currently stored   */
static idx_t cache_cap    = 0;   /* allocated capacity of the cache    */

static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;                 /* already big enough */

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n)
        new_cap *= 2;

    seq_t *tmp = NULL;
#if defined(__cplusplus)
    tmp = reinterpret_cast<seq_t*>(operator new[](new_cap * sizeof(seq_t)));
#else
    tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!tmp)
        abort();               /* allocation failure */

    /* copy existing values into the new buffer */
    for (idx_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];

    /* free old buffer */
    if (cache) {
#if defined(__cplusplus)
        operator delete[](cache);
#else
        free(cache);
#endif
    }

    cache = tmp;
    cache_cap = new_cap;
}

/* hard‑coded initial values – only the first five are needed */
static const seq_t init_vals[5] = { (seq_t)1, (seq_t)7, (seq_t)52, (seq_t)366, (seq_t)2563 };

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    ensure_capacity(n);

    /* compute missing values up to n (in any order of calls) */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 5) {
            cache[i - 1] = init_vals[i - 1];
        } else {
            cache[i - 1] =
                (seq_t)(7) * cache[i - 2]  /* 7*a(i-1)   */
              + cache[i - 5]               /* + a(i-4)    */
              - (seq_t)(7) * cache[i - 6]; /* - 7*a(i-5) */
        }
    }
    cache_size = n;
    return cache[n - 1];
}
