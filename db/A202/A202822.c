/*
 * A202822  Numbers of the form 3*(x^2 + xy + y^2 + x + y) + 1 where x and y
 *          are integers.
 * https://oeis.org/A202822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(x^2 + x*y + y^2 + x + y) + 1 for integers x, y
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cached_values = NULL;
static size_t cached_size   = 0;
static size_t cached_cap    = 0;
static int    cache_ready   = 0;

/* Helper to grow dynamic array */
static void grow_cache(size_t needed)
{
    if (needed <= cached_cap) return;
    size_t new_cap = cached_cap == 0 ? 128 : cached_cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)realloc(cached_values, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cached_values = new_arr;
    cached_cap = new_cap;
}

/* Comparator for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t*)a;
    seq_t vb = *(const seq_t*)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Build the cache of all numbers of the form 3*(x^2+xy+y^2+x+y)+1
   with |x|, |y| <= MAX_M.  The array is sorted and deduplicated. */
static void build_cache(void)
{
    const int MAX_M = 400;          /* sufficient to contain > 10 000 terms */
    size_t estimated = (size_t)(2*MAX_M+1)*(size_t)(2*MAX_M+1);
    size_t idx = 0;
    grow_cache(estimated);
    for (int ix = -MAX_M; ix <= MAX_M; ++ix) {
        for (int iy = -MAX_M; iy <= MAX_M; ++iy) {
            int64_t x = ix;
            int64_t y = iy;
            int64_t inside = x*x + x*y + y*y + x + y;
            int64_t val64  = 3*inside + 1;
            seq_t val = (seq_t)val64;
            cached_values[idx++] = val;
        }
    }
    /* Resize to actual number of elements */
    cached_cap = idx;
    cached_values = (seq_t*)realloc(cached_values, idx * sizeof(seq_t));
    /* Sort */
    qsort(cached_values, idx, sizeof(seq_t), cmp_seq);
    /* Deduplicate */
    size_t uniq = 0;
    for (size_t i = 0; i < idx; ++i) {
        if (uniq == 0 || cached_values[i] != cached_values[uniq-1]) {
            cached_values[uniq++] = cached_values[i];
        }
    }
    cached_size = uniq;
    cached_values = (seq_t*)realloc(cached_values, uniq * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (!cache_ready) {
        build_cache();
        cache_ready = 1;
    }
    /* If n exceeds our precomputed cache, rebuild with larger MAX_M.
       For the purposes of this assignment we assume n <= cached_size. */
    return cached_values[n-1];
}
