/*
 * A016111  Expansion of g.f.
 *          1/((1-11*x)*(1-12*x)*(1-13*x)*(1-14*x)*(1-15*x)).
 * https://oeis.org/A016111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 65*a(n-1) - 1685*a(n-2) + 21775*a(n-3) - 140274*a(n-4) +
 *            360360*a(n-5), a(0)=1, a(1)=65, a(2)=2540, a(3)=77350,
 *            a(4)=2022951
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;          /* number of values actually stored */
    /* pre‑computed first five terms */
    static const seq_t base[5] = {
        (seq_t)1,
        (seq_t)65,
        (seq_t)2540,
        (seq_t)77350,
        (seq_t)2022951
    };

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out‑of‑memory: return 0 */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        len = 5;
    }

    /* If the requested index is already computed, return it */
    if ((size_t)n < len)
        return cache[n];

    /* Grow the cache if necessary */
    size_t target = (size_t)n + 1;
    if (target > cap) {
        size_t new_cap = cap;
        while (new_cap < target)
            new_cap <<= 1;          /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out‑of‑memory: return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute values from the first missing index up to n */
    for (size_t i = len; i < target; ++i) {
        seq_t term1 = (seq_t)65  * cache[i-1];
        term1 += (seq_t)21775 * cache[i-3];
        term1 += (seq_t)360360 * cache[i-5];

        seq_t term2 = (seq_t)1685  * cache[i-2];
        term2 += (seq_t)140274 * cache[i-4];

        cache[i] = term1 - term2; /* safe subtraction: result is ≥0 */
    }
    len = target;
    return cache[n];
}
