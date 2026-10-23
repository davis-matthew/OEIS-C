/*
 * A049978  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), with a(1) = 1, a(2) = 3, and a(3) = 4.
 * https://oeis.org/A049978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1-2^ceil(-1+log2(n-1))) + Sum_{i=1}^{n-1} a(i);
 *            Initial values: a(1)=1, a(2)=3, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types are supplied by the build system:
   seq_t   – unsigned integer type of arbitrary width (uint8_t, uint32_t, __int128, …)
   idx_t   – unsigned 64‑bit integer type (uint64_t)
*/

static seq_t *cache = NULL;        /* 1‑based index, cache[0] unused */
static size_t cache_cap = 0;       /* current capacity (including cache[0]) */
static idx_t cache_max = 0;        /* largest n for which a(n) is stored */
static seq_t prefix_sum = 0;       /* sum of all a(i) computed so far */

/* Ensure the cache can hold at least indices 0..n */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise the first three terms if not already done */
static void init_cache(void)
{
    if (cache_max < 1) {
        ensure_cache_size(3);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache_max = 3;
        prefix_sum = cache[1] + cache[2] + cache[3];
    }
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    if (n == 1) {
        init_cache();
        return cache[1];
    }
    if (n == 2) {
        init_cache();
        return cache[2];
    }
    if (n == 3) {
        init_cache();
        return cache[3];
    }

    init_cache();

    if (n <= cache_max)
        return cache[n];

    /* Compute all terms up to n */
    ensure_cache_size(n);
    for (idx_t next = cache_max + 1; next <= n; ++next) {
        /* Largest power of two <= next-2 */
        idx_t x = next - 2;
        idx_t pow2 = 1;
        while ((pow2 << 1) <= x)
            pow2 <<= 1;

        idx_t m = next - 1 - pow2;           /* 1 <= m < next */
        seq_t a_m = cache[m];
        seq_t sum_prev = prefix_sum;         /* sum of a(1) .. a(next-1) */

        seq_t val = sum_prev + a_m;
        cache[next] = val;
        prefix_sum += val;
    }

    cache_max = n;
    return cache[n];
}
