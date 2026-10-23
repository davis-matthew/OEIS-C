/*
 * A169570  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) =
 *            27·(a(n-1)+…+a(n-34)) – 378·a(n-35) + p(n), where p(0)=1,
 *            p(1)…p(34)=2, p(35)=1, and p(n)=0 for n>35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(0) = 1, a(1) = 29, a(2) = 812, … by the above recurrence. */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* array of a(0), a(1), … */
static idx_t cache_len = 0;        /* number of values stored (next free index) */
static idx_t cache_cap = 0;        /* allocated capacity */

/* Ensure that cache has at least need+1 entries. */
static void ensure_cache(idx_t need)
{
    if (need < cache_len)
        return;

    if (cache_cap <= need) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= need)
            new_cap <<= 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                     /* out of memory */
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }
    /* Compute missing entries. */
    for (idx_t i = cache_len; i <= need; ++i) {
        seq_t sum_prev = 0;
        /* sum of the previous up to 34 terms */
        for (idx_t k = 1; k <= 34 && k <= i; ++k)
            sum_prev += cache[i - k];

        seq_t p_n;
        if (i == 0)
            p_n = 1;
        else if (i <= 34)
            p_n = 2;
        else if (i == 35)
            p_n = 1;
        else
            p_n = 0;

        seq_t val = 27 * sum_prev;
        if (i >= 35)
            val -= 378 * cache[i - 35];
        val += p_n;

        cache[i] = val;
    }
    cache_len = need + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
