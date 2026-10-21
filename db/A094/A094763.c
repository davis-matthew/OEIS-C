/*
 * A094763  Trajectory of 2 under repeated application of the map n -> n +
 *          square excess of n.
 * https://oeis.org/A094763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2; for n>0, a(n)=a(n-1)+a(n-1)-floor(sqrt(a(n-1)))^2  */
#include <stdlib.h>
#include <assert.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;  /* sequence values */
    static size_t cap     = 0;     /* allocated capacity */
    static idx_t  mx      = 0;     /* highest index computed */

    /* already computed? */
    if (n <= mx && cache != NULL)
        return cache[n];

    /* On first call we need to allocate and initialise a(0)=2 */
    if (cache == NULL) {
        size_t init_cap = (size_t)(n + 1);
        if (init_cap < 1)
            init_cap = 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = (seq_t)2;
        mx = 0;
        cap = init_cap;
    }
    else if (n + 1 > cap) {        /* grow cache if necessary */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        cap = new_cap;
    }

    /* grow from mx+1 up to n */
    seq_t value = cache[mx];
    for (idx_t i = mx + 1; i <= n; ++i) {
        seq_t sqrt_floor = floor_sqrt(value);    /* floor(sqrt(value)) */
        seq_t sq = sqrt_floor * sqrt_floor;      /* its square */
        value = value + (value - sq);            /* square excess addition */
        cache[i] = value;
    }
    mx = n;
    return cache[n];
}
