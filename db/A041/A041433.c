/*
 * A041433  Denominators of continued fraction convergents to sqrt(232).
 * https://oeis.org/A041433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 39206*a(n-6) - a(n-12) for n >= 12 Initial values:
 *            a(0)=1, a(1)=4, a(2)=13, a(3)=95, a(4)=298, a(5)=1287,
 *            a(6)=38908, a(7)=156919, a(8)=509665, a(9)=3724574,
 *            a(10)=11683387, a(11)=50458122
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[12] = {
    (seq_t)1,(seq_t)4,(seq_t)13,(seq_t)95,(seq_t)298,
    (seq_t)1287,(seq_t)38908,(seq_t)156919,(seq_t)509665,
    (seq_t)3724574,(seq_t)11683387,(seq_t)50458122
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_until = 0;   /* largest index for which cache[i] is valid */

seq_t a(idx_t n)
{
    /* Ensure cache large enough to hold a(n) */
    if (!cache || n + 1 > cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();          /* memory allocation failed */
        cache = tmp;
        cache_size = new_cap;
    }

    /* Make sure the first 12 values are stored */
    if (computed_until < 12 || cache_size > 12) {
        for (idx_t i = 0; i < 12 && i < cache_size; ++i)
            cache[i] = base_vals[i];
        computed_until = (cache_size > 12) ? 11 : cache_size - 1;
    }

    /* Compute missing values up to n */
    if (n > computed_until) {
        idx_t start = (computed_until < 11) ? 12 : computed_until + 1;
        for (idx_t i = start; i <= n; ++i)
            cache[i] = (seq_t)39206 * cache[i - 6] - cache[i - 12];
        computed_until = n;
    }

    return cache[n];
}
