/*
 * A023947  Expansion of g.f. 1/((1-x)*(1-5*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A023947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=28, a(2)=527, a(3)=8390,
 *            a(n)=28*a(n-1)-257*a(n-2)+830*a(n-3)-600*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;

/* Ensure that cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cap)
        return;

    size_t oldcap = cap;
    size_t newcap = cap ? cap * 2 : 4;
    while (newcap <= (size_t)n)
        newcap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)   /* allocation failure – unlikely for requested n */
        return;
    cache = tmp;
    cap = newcap;

    /* Zero the newly allocated region (safe sentinel) */
    for (size_t i = oldcap; i < newcap; ++i)
        cache[i] = 0;
}

/* Return the n-th term of A023947 */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Handle the first four terms directly */
    if (n < 4) {
        static const seq_t base[4] = {1, 28, 527, 8390};
        return base[n];
    }

    /* Initialize base values once */
    if (cache[0] == 0) {
        cache[0] = 1;
        cache[1] = 28;
        cache[2] = 527;
        cache[3] = 8390;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != 0)
            continue; /* already computed by a previous call */

        cache[i] = 28 * cache[i - 1]
                 - 257 * cache[i - 2]
                 + 830 * cache[i - 3]
                 - 600 * cache[i - 4];
    }

    return cache[n];
}
