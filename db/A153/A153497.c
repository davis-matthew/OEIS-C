/*
 * A153497  a(n) is the number whose binary expansion is A153498(n).
 * https://oeis.org/A153497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 14*a(n-2) + 50*a(n-3) - 56*a(n-4) + 80*a(n-5)
 *            - 64*a(n-6), a(1)=1, a(2)=7, a(3)=21, a(4)=73, a(5)=341,
 *            a(6)=1453
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* 1‑based cache, cache[0] unused */
static idx_t cache_capacity = 0;      /* total allocated slots (including 0)   */
static idx_t cache_size = 0;          /* highest n for which cache[n] is set   */
static const seq_t init_vals[6] = {
    (seq_t)1, (seq_t)7, (seq_t)21,
    (seq_t)73, (seq_t)341, (seq_t)1453
};

/* Ensure capacity for indices up to n (n ≥ 1). */
static void
ensure_cache(idx_t n)
{
    if (cache_capacity >= n + 1) return;   /* already big enough */

    /* Grow capacity, at least doubling each time. */
    idx_t new_capacity = (cache_capacity == 0) ? 8 : cache_capacity * 2;
    while (new_capacity < n + 1) new_capacity *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!new_cache) exit(1); /* allocation failure → abort */

#ifdef __cplusplus
    /* For class types: default‑construct each new element. */
    for (idx_t i = cache_capacity; i < new_capacity; ++i)
        new (new_cache + i) seq_t();
#else
    /* POD types: zero‑initialize new slots. */
    for (idx_t i = cache_capacity; i < new_capacity; ++i)
        new_cache[i] = (seq_t)0;
#endif
    cache = new_cache;
    cache_capacity = new_capacity;
}

/* Place first six terms into the cache (if not already done). */
static void
init_first6(void)
{
    if (cache_size >= 6) return;           /* already initialized */

    if (cache_capacity < 7)                /* need space for indices 1..6 */
        ensure_cache(6);

    cache[1] = init_vals[0];
    cache[2] = init_vals[1];
    cache[3] = init_vals[2];
    cache[4] = init_vals[3];
    cache[5] = init_vals[4];
    cache[6] = init_vals[5];

    /* The rest of the allocated slots remain uninitialized until needed. */
    cache_size = 6;
}

/* Sequence function. External linkage requested. */
seq_t
a(idx_t n)
{
    if (n <= 6) return init_vals[n - 1];   /* direct lookup of the first six */

    ensure_cache(n);
    init_first6();                        /* guarantee indices 1..6 present */

    if (cache_size < n) {
        idx_t start = cache_size + 1;      /* first index that is still unknown */
        if (start < 7) start = 7;          /* safety for the very first run */

        for (idx_t i = start; i <= n; ++i) {
            seq_t val = (seq_t)5 * cache[i - 1];
            val -= (seq_t)14 * cache[i - 2];
            val += (seq_t)50 * cache[i - 3];
            val -= (seq_t)56 * cache[i - 4];
            val += (seq_t)80 * cache[i - 5];
            val -= (seq_t)64 * cache[i - 6];
            cache[i] = val;
        }
        cache_size = n;
    }
    return cache[n];
}
