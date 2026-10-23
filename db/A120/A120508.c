/*
 * A120508  Generalized meta-Fibonacci sequence a(n) with parameters s=1 and
 *          k=4.
 * https://oeis.org/A120508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=1,2; a(n)=n-1 for 3<=n<=5; a(n)=a(n-1-a(n-1)) +
 *            a(n-2-a(n-2)) + a(n-3-a(n-3)) + a(n-4-a(n-4)) for n>5
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static seq_t *cache = NULL;     /* cache[0] unused, indices 1..n */
static size_t cache_cap = 0;    /* current capacity of cache (size_t) */

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (n + 1 > new_cap) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed; abort to avoid undefined behavior */
        abort();
    }
    /* zero the newly allocated part */
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

static seq_t get_value(idx_t i)
{
    if (i == 0) return 0;          /* should not happen for valid indices */
    /* recursion guard: if value already computed, return it */
    if (cache[i] != 0) return cache[i];

    seq_t val;
    if (i <= 2) {
        val = 1;
    } else if (i <= 5) {
        val = (seq_t)(i - 1);
    } else {
        /* compute recursive terms */
        idx_t sub1 = (idx_t)(i - 1) - (idx_t)get_value(i - 1);
        idx_t sub2 = (idx_t)(i - 2) - (idx_t)get_value(i - 2);
        idx_t sub3 = (idx_t)(i - 3) - (idx_t)get_value(i - 3);
        idx_t sub4 = (idx_t)(i - 4) - (idx_t)get_value(i - 4);
        seq_t v1 = get_value(sub1);
        seq_t v2 = get_value(sub2);
        seq_t v3 = get_value(sub3);
        seq_t v4 = get_value(sub4);
        val = v1 + v2 + v3 + v4;
    }
    cache[i] = val;
    return val;
}

seq_t a(idx_t n)
{
    /* enforce cache size */
    ensure_cache(n);
    /* compute and return */
    return get_value(n);
}
