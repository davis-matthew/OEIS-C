/*
 * A176952  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=0, k=-1 and l=-1.
 * https://oeis.org/A176952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 18, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from OEIS A176952:
   a(0)=1, a(1)=0, a(2)=-3, a(3)=-10, a(4)=-25.
   For n≥5:
      a(n) = -(((-7*n+2)*a(n-1)+(19*n-29)*a(n-2)
               +13*(2-n)*a(n-3)+4*(5-n)*a(n-4)
               +4*(n-5)*a(n-5)) /(n+1)     */
/* Handles any idx_t (uint64_t) and seq_t (signed integer or big‑int) */

#include <stdlib.h>

static seq_t *cache = NULL;       /* computed values */
static idx_t cache_cap = 0;       /* allocated size (in elements) */
static idx_t cache_last = 0;      /* greatest index computed */

/* make sure cache can hold indices 0..need */
static void
ensure_capacity(idx_t need)
{
    if (cache_cap > need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 2;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* if allocation fails we abort – the build is expected to have enough memory */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (!cache) {
        /* initialise with the first five terms */
        ensure_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-3;
        cache[3] = (seq_t)-10;
        cache[4] = (seq_t)-25;
        cache_last = 4;
    }

    if (n <= cache_last)
        return cache[n];

    for (idx_t m = cache_last + 1; m <= n; ++m) {
        ensure_capacity(m);
        if (m <= 4) {                /* safety – should never hit, but keep init */
            static const seq_t init[] = {(seq_t)1,(seq_t)0,(seq_t)-3,(seq_t)-10,(seq_t)-25};
            cache[m] = init[m];
            continue;
        }
        long long tt = (long long)m;   /* cast for signed arithmetic */
        seq_t c1 = (seq_t)((-7LL)*tt + 2);
        seq_t c2 = (seq_t)(19LL*tt - 29);
        seq_t c3 = (seq_t)(13LL*(2 - tt));
        seq_t c4 = (seq_t)(4LL*(5 - tt));
        seq_t c5 = (seq_t)(4LL*(tt - 5));
        seq_t denom = (seq_t)(tt + 1);
        seq_t num = -(c1*cache[m-1] + c2*cache[m-2] + c3*cache[m-3] + c4*cache[m-4] + c5*cache[m-5]);
        cache[m] = num / denom;
    }
    cache_last = n;
    return cache[n];
}
