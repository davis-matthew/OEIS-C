/*
 * A224131  Number of n X 6 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 9*a(n-3) + 4*a(n-5) + 16*a(n-6)
 *            with offset 1 (a(1)=7, a(2)=49, a(3)=269, a(4)=1279, a(5)=5648,
 *            a(6)=24095)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* The function definition */
seq_t a(idx_t n)
{
    /* Return zero for indices below the offset (undefined behaviour) */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for previously computed terms */
    static seq_t *cache       = NULL;   /* 1‑based cache array          */
    static size_t capacity    = 0;      /* allocated array capacity      */
    static idx_t  computed    = 0;      /* highest index computed so far */
    
    /* Initialize cache on first call */
    if (cache == NULL) {
        capacity = 16;                 /* initial capacity */
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (cache == NULL)            /* out of memory – abort */
            abort();
        /* zero the array to avoid garbage */
        for (size_t i = 0; i < capacity; ++i)
            cache[i] = (seq_t)0;
    }

    /* Grow the array if it cannot contain index n */
    while (n >= capacity) {
        size_t new_cap = capacity << 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL)
            abort();
        /* initialize newly allocated elements to zero */
        for (size_t i = capacity; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache    = tmp;
        capacity = new_cap;
    }

    /* Ensure the first six terms are defined */
    if (computed < 1) { cache[1] = (seq_t)7;   computed = 1; }
    if (computed < 2) { cache[2] = (seq_t)49;  computed = 2; }
    if (computed < 3) { cache[3] = (seq_t)269; computed = 3; }
    if (computed < 4) { cache[4] = (seq_t)1279;computed = 4; }
    if (computed < 5) { cache[5] = (seq_t)5648;computed = 5; }
    if (computed < 6) { cache[6] = (seq_t)24095;computed = 6; }

    /* Compute missing terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)7   * cache[i-1];
        val -= (seq_t)14   * cache[i-2];
        val += (seq_t)9    * cache[i-3];
        val += (seq_t)4    * cache[i-5];
        val += (seq_t)16   * cache[i-6];
        cache[i] = val;
    }
    computed = n;

    return cache[n];
}
