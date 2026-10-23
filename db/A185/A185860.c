/*
 * A185860  1/128 the number of (n+2)X5 binary arrays with no 3X3 subblock
 *          trace equal to any horizontal or vertical neighbor 3X3 subblock
 *          trace.
 * https://oeis.org/A185860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*a(n-1) + 37*a(n-2) - 181*a(n-3) + 36*a(n-4)
 *             + 264*a(n-5) - 119*a(n-6) + 27*a(n-7) - 49*a(n-8)
 *             + 25*a(n-9) - 16*a(n-10)   (n > 11)
 * Initial values: a(1)=124, a(2)=1254, a(3)=13662, a(4)=148747,
 *                 a(5)=1625738, a(6)=17735956, a(7)=193660943,
 *                 a(8)=2113733736, a(9)=23075049417,
 *                 a(10)=251881260545, a(11)=2749586366993.
 * The implementation stores computed values in a static cache.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* cached sequence values */
static size_t cache_cap  = 0;      /* allocated capacity of the cache */
static idx_t  cache_len  = 0;      /* highest index that has a valid value */

/* Ensure the cache can hold indices up to `need`. */
static void ensure_cache(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= (size_t)need + 1)
        new_cap <<= 1;   /* double until enough */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();        /* allocation failed */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Fill the cache with the known initial values (up to a(11)). */
static void init_cache(void)
{
    if (!cache) {
        ensure_cache(11);
        cache[0] = (seq_t)0;            /* unused */
        cache[1] = (seq_t)124;
        cache[2] = (seq_t)1254;
        cache[3] = (seq_t)13662;
        cache[4] = (seq_t)148747;
        cache[5] = (seq_t)1625738;
        cache[6] = (seq_t)17735956;
        cache[7] = (seq_t)193660943;
        cache[8] = (seq_t)2113733736;
        cache[9] = (seq_t)23075049417;
        cache[10] = (seq_t)251881260545;
        cache[11] = (seq_t)2749586366993;
        cache_len = 11;
    } else {
        /* In case some of the initial entries were not set yet. */
        if (cache_len < 11) {
            if (cache_cap < 12)
                ensure_cache(11);
            if (cache[1] == (seq_t)0) {
                cache[1] = (seq_t)124;
                cache[2] = (seq_t)1254;
                cache[3] = (seq_t)13662;
                cache[4] = (seq_t)148747;
                cache[5] = (seq_t)1625738;
                cache[6] = (seq_t)17735956;
                cache[7] = (seq_t)193660943;
                cache[8] = (seq_t)2113733736;
                cache[9] = (seq_t)23075049417;
                cache[10] = (seq_t)251881260545;
                cache[11] = (seq_t)2749586366993;
            }
            if (cache_len < 11)
                cache_len = 11;
        }
    }
}

/* Multiply a sequence value by a small constant (≤264). */
static inline seq_t mul_const(seq_t v, unsigned m)
{
    return v * (seq_t)m;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0; guard against caller bugs */
        return 0;

    init_cache();            /* guarantee that a(1)..a(11) exist */
    ensure_cache(n);         /* make room for indices up to n */

    if (n <= cache_len && cache[n] != (seq_t)0)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = mul_const(cache[i-1], 9)  + mul_const(cache[i-2], 37)
                  + mul_const(cache[i-4], 36) + mul_const(cache[i-5], 264)
                  + mul_const(cache[i-7], 27) + mul_const(cache[i-9], 25);
        seq_t neg = mul_const(cache[i-3], 181) + mul_const(cache[i-6], 119)
                  + mul_const(cache[i-8], 49)  + mul_const(cache[i-10], 16);
        cache[i] = pos - neg;
    }

    cache_len = n;
    return cache[n];
}
