/*
 * A187003  Expansion of 1/(1-x-2*x^2-3*x^3-3*x^4-2*x^5-x^6).
 * https://oeis.org/A187003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)+3*a(n-3)+3*a(n-4)+2*a(n-5)+a(n-6) for
 *            n>=6 with a(0)=1,a(1)=1,a(2)=3,a(3)=8,a(4)=20,a(5)=50
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;
    if (cache == NULL)
        cache = (seq_t*)realloc(NULL, (n + 1) * sizeof(seq_t));
    else
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    /* fill missing entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else if (i == 3)
            cache[i] = (seq_t)8;
        else if (i == 4)
            cache[i] = (seq_t)20;
        else if (i == 5)
            cache[i] = (seq_t)50;
        else {
            seq_t next = cache[i - 1];
            next += (seq_t)2 * cache[i - 2];
            next += (seq_t)3 * cache[i - 3];
            next += (seq_t)3 * cache[i - 4];
            next += (seq_t)2 * cache[i - 5];
            next += cache[i - 6];
            cache[i] = next;
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
