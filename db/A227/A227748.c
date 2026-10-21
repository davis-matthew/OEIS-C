/*
 * A227748  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having determinant equal to one.
 * https://oeis.org/A227748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227748 – Number of n×3 0,1 arrays indicating 2×2 subblocks of some larger
   (n+1)×4 binary array having determinant equal to one.
   Recurrence (order 13):
   a(n) = a(n-1)+12*a(n-2)+44*a(n-3)+71*a(n-4)+78*a(n-5)+40*a(n-6)
          -7*a(n-7)-33*a(n-8)-14*a(n-9)+11*a(n-10)-a(n-11)-4*a(n-12)+a(n-13)
   Initial values: a(1)=7, a(2)=40, a(3)=210, a(4)=1122, a(5)=6002, a(6)=32114,
   a(7)=171759, a(8)=918746, a(9)=4914405, a(10)=26287020,
   a(11)=140609297, a(12)=752118530, a(13)=4023077949
*/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, realloc, free */

static seq_t *cache = NULL;      /* dynamic cache of computed values */
static size_t cache_cap = 0;     /* capacity of cache array (in elements) */
static idx_t cached_max = 0;     /* largest n that has been computed */

/* explicit initial values up to a(13) */
static const seq_t init_vals[14] = {
    /* 0 unused to align OEIS offset (a(1) is first term) */
    (seq_t)0ull,
    (seq_t)7ull, (seq_t)40ull, (seq_t)210ull, (seq_t)1122ull, (seq_t)6002ull,
    (seq_t)32114ull, (seq_t)171759ull, (seq_t)918746ull, (seq_t)4914405ull,
    (seq_t)26287020ull, (seq_t)140609297ull, (seq_t)752118530ull,
    (seq_t)4023077949ull
};

/* ensure that cache has room for indices 0 … n */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0 … n inclusive */
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)   /* allocation failed – abort */
    {
        /* In a competitive setting we might exit or raise an error.
           Here we simply abort to keep the interface stable. */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) for n >= 1, memoised */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; return 0 for out‑of‑range */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL)
    {
        /* ensure capacity for the first 13 terms; also allow n + 1 space */
        size_t initial_cap = (size_t)n + 1;
        if (initial_cap < 14)
            initial_cap = 14;
        cache = (seq_t *)malloc(initial_cap * sizeof(seq_t));
        if (!cache)
            abort();

        cache_cap = initial_cap;

        for (size_t i = 1; i <= 13; ++i)
            cache[i] = init_vals[i];

        cached_max = 13;
    }

    /* if n already computed, return immediately */
    if (n <= cached_max)
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    ensure_capacity(n);

    /* compute missing terms using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        size_t idx = (size_t)i;
        seq_t val = cache[idx-1]; /* a(i-1) */
        val += (seq_t)12 * cache[idx-2];
        val += (seq_t)44 * cache[idx-3];
        val += (seq_t)71 * cache[idx-4];
        val += (seq_t)78 * cache[idx-5];
        val += (seq_t)40 * cache[idx-6];
        val -= (seq_t)7  * cache[idx-7];
        val -= (seq_t)33 * cache[idx-8];
        val -= (seq_t)14 * cache[idx-9];
        val += (seq_t)11 * cache[idx-10];
        val -= cache[idx-11];
        val -= (seq_t)4  * cache[idx-12];
        val += cache[idx-13];
        cache[idx] = val;
    }

    cached_max = n;
    return cache[(size_t)n];
}
