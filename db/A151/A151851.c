/*
 * A151851  a(0)=0; a(1)=0; a(2)=0; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=a(j) + 2*a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=0, a(2)=0; for n>=3 let i=floor(log2 n), j=n-2^i,
 *            then a(n)=a(j)+2*a(j+1) (+1 if j=2^i-1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Compute values up to n inclusive (if not already done) */
static void compute_up_to(idx_t n)
{
    /* ensure cache allocated and initialized */
    if (cache_len == 0) {
        size_t init_len = 3;      /* indices 0,1,2 */
        cache = (seq_t*)calloc(init_len, sizeof(seq_t));
        cache_len = init_len;
    }

    if (n < cache_len)
        return;        /* already computed */

    /* enlarge cache */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    cache = tmp;
    /* initialize new entries to zero */
    for (size_t i = old_len; i < new_len; ++i)
        cache[i] = 0;
    cache_len = new_len;

    /* compute new entries */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = 0;
            continue;
        }
        /* find highest power of two <= i */
        idx_t p = 1;
        while ((p << 1) <= i)
            p <<= 1;
        idx_t j = i - p;
        seq_t val = cache[j] + (cache[j + 1] * 2);
        if (j == p - 1)
            val += 1;
        cache[i] = val;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
