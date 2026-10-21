/*
 * A228364  G.f.: x^2*(x+1)^2/(x^3+x^2-1)^2.
 * https://oeis.org/A228364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=6;
 *            a(n)=2*a(n-2)+2*a(n-3)-a(n-4)-2*a(n-5)-a(n-6) for n>5
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_size(idx_t n)
{
    /* grow cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap < 8 ? 8 : cache_cap;
        while (new_cap < required) new_cap <<= 1;
        seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();          /* allocation failed */
        cache = newp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)0;
        } else if (i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)2;
        } else if (i == 4) {
            cache[i] = (seq_t)3;
        } else if (i == 5) {
            cache[i] = (seq_t)6;
        } else {
            /* compute using recurrence in a safe way:
               a(n) = 2*a(n-2) + 2*a(n-3) - [a(n-4) + 2*a(n-5) + a(n-6)] */
            seq_t sum = cache[i-2] * ((seq_t)2) + cache[i-3] * ((seq_t)2);
            seq_t sub = cache[i-4] + cache[i-5] * ((seq_t)2) + cache[i-6];
            cache[i] = sum - sub;
        }
    }
    cache_len = required;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return cache[(size_t)n];
}
