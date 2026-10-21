/*
 * A052720  Expansion of e.g.f.: (1 - 4*x + 3*x^2)*(1 - 2*x - sqrt(1-4*x))/2 -
 *          x^2 + 2*x^3.
 * https://oeis.org/A052720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*A003517(n-4) Base values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=720 Recurrence for n>=7:
 *            a(n) = a(n-1)*(n-4)*2*(2*n-7)/(n-6)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* stored values of the sequence */
static size_t cache_cap = 0;          /* allocated capacity of the array   */
static size_t cache_len = 0;          /* number of entries already computed */

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;      /* indices 0..n inclusive */

    /* enlarge the array if necessary */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();        /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* compute all missing entries */
    for (size_t i = cache_len; i < need; ++i) {
        if (i <= 5) {
            cache[i] = 0;
        } else if (i == 6) {
            cache[i] = (seq_t)720;
        } else {
            idx_t idx = i;                            /* current index n */
            /* recurrence: a(n) = a(n-1)*(n-4)*2*(2*n-7)/(n-6) */
            seq_t prev = cache[i - 1];
            seq_t num = prev * (seq_t)(idx - 4);
            num = num * 2;
            num = num * (seq_t)(2 * idx - 7);
            cache[i] = num / (seq_t)(idx - 6);
        }
    }
    cache_len = need;        /* update length after filling */
    return cache[(size_t)n];
}
