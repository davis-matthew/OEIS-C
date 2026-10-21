/*
 * A200409  The y-values in the solution to 19*x^2 - 18 = y^2.
 * https://oeis.org/A200409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 340*a(n-3) - a(n-6); initial values: a(1)=1, a(2)=39,
 *            a(3)=571, a(4)=911, a(5)=13299, a(6)=194141
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamically allocated cache */
static idx_t cache_size = 0;     /* number of terms currently stored (max n already computed) */
static idx_t cache_cap = 0;      /* allocated capacity of cache */

/* Ensure that cache has capacity for at least n terms */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < n) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Store the first six initial terms if they are not already present */
static void init_initial(void)
{
    if (cache_size >= 6) return;
    ensure_capacity(6);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)39;
    cache[2] = (seq_t)571;
    cache[3] = (seq_t)911;
    cache[4] = (seq_t)13299;
    cache[5] = (seq_t)194141;
    cache_size = 6;
}

/* Sequence function required by the specification */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n=0, return 0 as safe fallback */

    ensure_capacity(n);

    if (n <= cache_size)     /* value already computed */
        return cache[n - 1];

    init_initial();           /* guarantees first six terms are present */

    /* Compute terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 6) continue; /* already set by init_initial */
        seq_t t3 = cache[i - 3 - 1];
        seq_t t6 = cache[i - 6 - 1];
        cache[i - 1] = (seq_t)340 * t3 - t6;
    }

    cache_size = n;
    return cache[n - 1];
}
