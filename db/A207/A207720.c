/*
 * A207720  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) +37*a(n-2) -3*a(n-3) -132*a(n-4) +26*a(n-5)
 *            +132*a(n-6) -59*a(n-7) -15*a(n-8) +6*a(n-9) + a(n-10) Initial
 *            values: a(1)=14; a(2)=196; a(3)=1414; a(4)=11970; a(5)=97580;
 *            a(6)=805154; a(7)=6614706; a(8)=54438356; a(9)=447687702;
 *            a(10)=3682876386
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/* Constant initial values */
static const seq_t base_vals[11] = {
    (seq_t)0,
    (seq_t)14,
    (seq_t)196,
    (seq_t)1414,
    (seq_t)11970,
    (seq_t)97580,
    (seq_t)805154,
    (seq_t)6614706,
    (seq_t)54438356,
    (seq_t)447687702,
    (seq_t)3682876386
};

/* Cached sequence values */
static seq_t *cache = NULL;      /* allocated array (index 0 unused) */
static idx_t cache_cap = 0;      /* current capacity (in indices) */
static idx_t cache_len = 0;      /* highest computed index */

/* Ensure the cache has room for indices up to 'needed' (inclusive) */
static void ensure_cache(idx_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = (size_t)needed + 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(new_ptr != NULL);
        cache = new_ptr;
        cache_cap = needed;
    }
}

/* Initialize the cache with the first ten terms (if not already done) */
static void init_cache()
{
    if (cache_cap < 10) {
        ensure_cache(10);
    }
    if (cache_len < 10) {
        for (idx_t i = 1; i <= 10; ++i) {
            cache[i] = base_vals[i];
        }
        cache_len = 10;
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined by OEIS, but safe default */
    }

    if (n <= 10) {
        return base_vals[n];
    }

    init_cache();          /* guarantees cache[1..10] are set */
    ensure_cache(n);       /* allocate space up to n */

    /* Compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Sum of positive terms */
        seq_t sum_pos = ((seq_t)4) * cache[i-1]
                      + ((seq_t)37) * cache[i-2]
                      + ((seq_t)26) * cache[i-5]
                      + ((seq_t)132) * cache[i-6]
                      + ((seq_t)6) * cache[i-9]
                      + cache[i-10];
        /* Sum of negative terms */
        seq_t sum_neg = ((seq_t)3) * cache[i-3]
                      + ((seq_t)132) * cache[i-4]
                      + ((seq_t)59) * cache[i-7]
                      + ((seq_t)15) * cache[i-8];
        cache[i] = sum_pos - sum_neg;
    }
    cache_len = n;
    return cache[n];
}
