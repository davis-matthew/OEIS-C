/*
 * A186873  Number of (n+2)X3 0..3 arrays with every 3X3 subblock commuting
 *          with each horizontal and vertical neighbor 3X3 subblock.
 * https://oeis.org/A186873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3) - 6*a(n-4) + 3*a(n-5) +
 *            3*a(n-6) - 3*a(n-7) for n > 20. Initial values (for n = 1..20):
 *            a(1)=262144, a(2)=2428, a(3)=19236, a(4)=46583, a(5)=104701,
 *            a(6)=186096, a(7)=277659, a(8)=322910, a(9)=569446,
 *            a(10)=975469, a(11)=1428461, a(12)=2729288, a(13)=4782807, a...
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 69, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types seq_t and idx_t are supplied by the build environment. */

static seq_t init[21] = {
    0ULL,            /* dummy for index 0 */
    262144ULL,       /* a(1) */
    2428ULL,         /* a(2) */
    19236ULL,        /* a(3) */
    46583ULL,        /* a(4) */
    104701ULL,       /* a(5) */
    186096ULL,       /* a(6) */
    277659ULL,       /* a(7) */
    322910ULL,       /* a(8) */
    569446ULL,       /* a(9) */
    975469ULL,       /* a(10) */
    1428461ULL,      /* a(11) */
    2729288ULL,      /* a(12) */
    4782807ULL,      /* a(13) */
    7112898ULL,      /* a(14) */
    12726103ULL,     /* a(15) */
    21815463ULL,     /* a(16) */
    33093523ULL,     /* a(17) */
    58123410ULL,     /* a(18) */
    99742321ULL,     /* a(19) */
    154917607ULL     /* a(20) */
};

seq_t a(idx_t n)
{
    /* Static cache to allow out‑of‑order calls and O(n) growth. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_cached = 0;

    if (n == 0) {
        /* The sequence is defined only for n >= 1; behaviour below offset is irrelevant. */
        return 0;
    }

    /* Allocate cache if not yet done. */
    if (cache == NULL) {
        capacity = 32;                /* initial capacity (>=20) */
        cache = (seq_t *)malloc(sizeof(seq_t) * (capacity + 1)); /* +1 for 1‑based indexing */
        if (!cache) return 0;         /* out of memory */
        /* Initialize known values. */
        for (idx_t i = 1; i <= 20 && i <= capacity; ++i)
            cache[i] = init[i];
        max_cached = (n <= 20) ? n : 20;
    }

    /* Resize cache if needed. */
    if (n > capacity) {
        size_t new_cap = capacity;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) return 0;          /* realloc failed */
        cache = tmp;
        capacity = new_cap;
    }

    if (n <= max_cached)
        return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 20) {
            cache[i] = init[i];
        } else {
            seq_t t1 = 2 * cache[i - 1];
            t1 += 3 * cache[i - 3];
            t1 += 3 * cache[i - 5];
            t1 += 3 * cache[i - 6];

            seq_t t2 = cache[i - 2];
            t2 += 6 * cache[i - 4];
            t2 += 3 * cache[i - 7];

            cache[i] = t1 - t2;
        }
    }
    max_cached = n;
    return cache[n];
}
