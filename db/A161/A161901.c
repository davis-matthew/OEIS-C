/*
 * A161901  Array read by rows in which row n lists the divisors of n, but if
 *          n is a square then the square root of n appears twice.
 * https://oeis.org/A161901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   A161901 - array read by rows: row n lists divisors of n, but if
 *            n is a square then the square root of n appears twice, read
 *            linearly starting from a(1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be generated on demand by caching previously
   computed values. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* integer square root (floor), small n only */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* comparator for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* cache for computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t current_row = 0;  /* last row fully processed */

/* ensure capacity of cache */
static void ensure_cache_capacity(size_t min_need)
{
    if (min_need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < min_need) new_cap = min_need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed; abort */ abort(); }
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safe */
    while (cache_len < (size_t)n) {
        ++current_row;
        /* gather divisors of current_row */
        uint64_t divs[128];
        int div_cnt = 0;
        for (uint64_t i = 1; i * i <= (uint64_t)current_row; ++i) {
            if (current_row % i == 0) {
                divs[div_cnt++] = i;
                if (i != current_row / i)
                    divs[div_cnt++] = current_row / i;
            }
        }
        /* sort divisors */
        qsort(divs, (size_t)div_cnt, sizeof(uint64_t), cmp_uint64);

        /* duplicate sqrt if square */
        uint64_t sqrt_val = isqrt_uint64((uint64_t)current_row);
        int dup_done = 0;
        int is_square = (sqrt_val * sqrt_val == (uint64_t)current_row);

        for (int j = 0; j < div_cnt; ++j) {
            uint64_t val = divs[j];
            ensure_cache_capacity(cache_len + 2); /* allow space for duplicate */
            cache[cache_len++] = (seq_t)val;
            if (is_square && !dup_done && val == sqrt_val) {
                cache[cache_len++] = (seq_t)val; /* duplicate sqrt */
                dup_done = 1;
            }
        }
    }
    return cache[n - 1];
}
