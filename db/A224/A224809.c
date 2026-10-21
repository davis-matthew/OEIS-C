/*
 * A224809  Number of permutations (p(1), p(2), ..., p(n)) satisfying -k <=
 *          p(i)-i <= r and p(i)-i not in the set I, i=1..n, with k=2, r=4,
 *          I={-1,1,2,3}.
 * https://oeis.org/A224809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) + a(n-6) - a(n-9);
 *            Initial values:
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=2,a(6)=4,a(7)=6,a(8)=9
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 234, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[9] = {1,1,1,1,1,2,4,6,9};

static seq_t *cache   = NULL;  /* array of computed values    */
static size_t  cap    = 0;      /* allocated capacity (in elements)  */
static size_t  used   = 0;      /* number of elements actually used   */

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n < 9u) return base_vals[(size_t)n];

    /* initialize cache if not yet done */
    if (cache == NULL) {
        cap = 9;
        used = 9;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 9; ++i) cache[i] = base_vals[i];
        if (cache == NULL) return (seq_t)0;  /* allocation failed */
    }

    /* if we need to extend the cache */
    if ((size_t)n + 1 > used) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t new_cap = cap;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;            /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }
        /* compute new terms */
        for (size_t i = used; i < need; ++i) {
            seq_t sum_pos = cache[i-1];
            sum_pos += cache[i-3];
            sum_pos += cache[i-5];
            sum_pos += cache[i-6];
            seq_t sum_neg = cache[i-4];
            sum_neg += cache[i-9];
            cache[i] = sum_pos - sum_neg;
        }
        used = need;
    }

    return cache[(size_t)n];
}
