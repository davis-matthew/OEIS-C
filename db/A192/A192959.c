/*
 * A192959  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) with
 *            a(0)=0, a(1)=1, a(2)=0, a(3)=3, a(4)=10
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_size = 0;     /* currently allocated size */
static idx_t computed_max = 0;   /* largest index computed so far */
static bool initialized = false;

/* Ensure the cache can hold at least need+1 elements */
static void grow_cache(idx_t need)
{
    if (cache_size > need)
        return;
    size_t new_cap = cache_size ? cache_size : 16;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_size = new_cap;
}

/* Initialise first five terms */
static void init_cache(void)
{
    grow_cache(4);
    cache[0] = 0;
    cache[1] = 1;
    cache[2] = 0;
    cache[3] = 3;
    cache[4] = 10;
    computed_max = 4;
    initialized = true;
}

/* Compute the sequence up to index n (inclusive) */
static void ensure_computed_up_to(idx_t n)
{
    if (n <= computed_max)
        return;
    grow_cache(n);
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i-1]
                 - 5 * cache[i-2]
                 + cache[i-3]
                 + 2 * cache[i-4]
                 - cache[i-5];
    }
    computed_max = n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (!initialized)
        init_cache();
    if (n > computed_max)
        ensure_computed_up_to(n);
    return cache[n];
}
