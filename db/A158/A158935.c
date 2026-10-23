/*
 * A158935  a(n)= -3a(n-1)-3a(n-2)-2a(n-3), n>3. a(0)=4, a(1)=4, a(2)=-5,
 *          a(3)=4.
 * https://oeis.org/A158935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1)-3*a(n-2)-2*a(n-3) a(0)=4, a(1)=4, a(2)=-5,
 *            a(3)=4
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 31, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t sz = 0;
static idx_t cap = 0;

static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cap)
        return;
    idx_t new_cap = cap > 0 ? cap : 4;
    while (new_cap <= n)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap](); /* value‑initialized */
    for (idx_t i = 0; i < sz; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    cache = new_cache;
#else
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache = new_cache;
#endif
    cap = new_cap;
}

static void init_cache(void)
{
    if (cache != NULL)
        return;
    cap = 4;
    sz = 4;
#ifdef __cplusplus
    cache = new seq_t[cap]();  /* zero‑initialized */
#else
    cache = (seq_t *)malloc(sizeof(seq_t) * cap);
#endif
    cache[0] = (seq_t)4;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)-5;
    cache[3] = (seq_t)4;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n < sz)
        return cache[n];
    ensure_capacity(n);
    for (idx_t i = sz; i <= n; ++i)
    {
        seq_t val = -(seq_t)3 * cache[i - 1]
                    - (seq_t)3 * cache[i - 2]
                    - (seq_t)2 * cache[i - 3];
        cache[i] = val;
    }
    sz = n + 1;
    return cache[n];
}
