/*
 * A164071  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(0)=1, a(1)=38, a(2)=1406,
 *            a(3)=52022, a(4)=1924814, a(5)=71218118, a(6)=2635069663 and for
 *            n≥7: a(n) = 36 a(n‑1)+36 a(n‑2)+36 a(n‑3)+36 a(n‑4)+36 a(n‑5) –
 *            666 a(n‑6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* next index to compute */

static void ensure_cap(idx_t n)
{
    if (cache_cap <= (size_t)n + 1) {
        size_t new_cap = 2 * ((size_t)n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

static void compute_up_to(idx_t n)
{
    static const seq_t init[7] = {
        (seq_t)1,
        (seq_t)38,
        (seq_t)1406,
        (seq_t)52022,
        (seq_t)1924814,
        (seq_t)71218118,
        (seq_t)2635069663
    };
    if (cache_len == 0) {
        /* fresh cache: store initial values up to min(n,6) */
        size_t upto = (size_t)(n < 6 ? n : 6);
        for (size_t i = 0; i <= upto; ++i) cache[i] = init[i];
        cache_len = upto + 1;
    }
    /* compute further terms if needed */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            seq_t t = ((seq_t)36 * cache[i-1])
                    + ((seq_t)36 * cache[i-2])
                    + ((seq_t)36 * cache[i-3])
                    + ((seq_t)36 * cache[i-4])
                    + ((seq_t)36 * cache[i-5])
                    - ((seq_t)666 * cache[i-6]);
            cache[i] = t;
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    ensure_cap(n);
    compute_up_to(n);
    return cache[(size_t)n];
}
