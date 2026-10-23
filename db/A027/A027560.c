/*
 * A027560  Number of 5-balanced strings of length n: let d(S)= #(1)'s in S -
 *          #(0)'s, then S is k-balanced if every substring T has -k<=d(T)<=k;
 *          here k=5.
 * https://oeis.org/A027560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2) - 4*a(n-3) - 6*a(n-4) + 3*a(n-5);
 *            Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32
 *            (valid for n ≥ 0)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* memoised sequence values */
static idx_t cache_len = 0;           /* current cached length (number of values) */

seq_t a(idx_t n)
{
    /* constants for the first 6 terms */
    static const seq_t base[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                   (seq_t)8, (seq_t)16, (seq_t)32 };

    /* ensure cache has at least the first 6 values */
    if (cache_len < 6) {
        idx_t new_len = 6;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp && new_len != 0) abort();   /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_len; i < 6; ++i) cache[i] = base[i];
        cache_len = 6;
    }

    /* if the requested n is already cached, return it immediately */
    if (n < cache_len) return cache[n];

    /* grow the cache to at least n+1 entries */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp && new_len != 0) abort();     /* allocation failure */
    cache = tmp;

    /* compute values from cache_len up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence is valid for i >= 6; for i < 6 we already have base */
        if (i < 6) continue;
        seq_t term1 = cache[i-1];
        seq_t term2 = cache[i-2];
        seq_t term3 = cache[i-3];
        seq_t term4 = cache[i-4];
        seq_t term5 = cache[i-5];

        /* compute a(i) = term1 + 5*term2 - 4*term3 - 6*term4 + 3*term5 */
        /* do additions first to keep intermediate unsigned arithmetic safe */
        seq_t sum_pos = term1 + term2 * ((seq_t)5) + term5 * ((seq_t)3);
        seq_t sum_neg = term3 * ((seq_t)4) + term4 * ((seq_t)6);
        cache[i] = sum_pos - sum_neg;
    }

    cache_len = new_len;
    return cache[n];
}
