/*
 * A016320  Expansion of 1/((1-2x)(1-8x)(1-12x)).
 * https://oeis.org/A016320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (54*12^n + 2^n - 40*8^n)/15
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=22; a(n)=20*a(n-1)-96*a(n-2)+2^n. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;
static seq_t pow2_current;

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    ensure_cache_cap(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)22;
    max_computed = 1;
    pow2_current = (seq_t)2;   /* 2^1 */
}

seq_t a(idx_t n)
{
    if (!cache)
        init_cache();

    if (n == 0)
        return cache[0];
    if (n == 1)
        return cache[1];

    ensure_cache_cap((size_t)n + 1);

    if (n > max_computed) {
        seq_t prev2 = cache[max_computed - 1];  /* a(n-2) */
        seq_t prev1 = cache[max_computed];      /* a(n-1) */
        /* current pow2_current holds 2^max_computed */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            pow2_current = pow2_current * (seq_t)2;          /* 2^i */
            seq_t curr = ((seq_t)20) * prev1 - ((seq_t)96) * prev2 + pow2_current;
            cache[i] = curr;
            prev2 = prev1;
            prev1 = curr;
        }
        max_computed = n;
    }

    return cache[n];
}
