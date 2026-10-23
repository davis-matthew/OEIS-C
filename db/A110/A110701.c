/*
 * A110701  Friendly run sums: numbers S with two run sums (sum of positive
 *          integer runs) that share one common number, i.e., S = a + (a+1) +
 *          ... + b = b + (b+1) + ... + c with a < b < c.
 * https://oeis.org/A110701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest positive integer that can be written as sum
 *            of two consecutive integer runs sharing one common number (the
 *            “friendly run sum”). The implementation enumerates all integer
 *            triples (a,b,c) with 0 < a < b < c that satisfy (b-a+1)(a+b) =
 *            (c-b+1)(b+c), computes the common sum S = (b...
 * Range:     u8 none, u16 none, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq.h"   /* cmp_seq for qsort */

/* static cache of unique sums */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* build the cache until it contains at least n terms */
static void build_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;          /* already enough */

    /* if we already have a cache, free it to rebuild */
    free(cache);
    cache = NULL;
    cache_len = 0;

    /* starting bound for u and v */
    idx_t max_uv = 5000;

    /* if needed > expected with this bound, double until enough */
    while (1) {
        /* temporary buffer to gather all found sums */
        size_t temp_cap = 8192;
        seq_t *temp = (seq_t *)malloc(temp_cap * sizeof(seq_t));
        if (!temp) return;   /* allocation failure: give up */
        size_t temp_len = 0;

        /* enumerate all (u,v) pairs */
        for (idx_t u = 2; u <= max_uv; ++u) {
            for (idx_t v = 2; v <= max_uv; ++v) {
                if (u == v) continue;

                /* compute numerator and denominator of b */
                int64_t num = (int64_t)u * (u - 1) + (int64_t)v * (v - 1);
                int64_t den = (int64_t)2 * (int64_t)(u - v);
                if (den == 0) continue;
                /* normalize sign */
                if (den < 0) { den = -den; num = -num; }

                if (num % den != 0) continue;     /* b not integer */

                int64_t b = num / den;
                if (b <= 0) continue;

                int64_t a = b - u + 1;
                int64_t c = b + v - 1;
                if (a <= 0 || c <= b) continue;   /* ensure a<b<c */

                /* compute the common sum: S = u*(a+b)/2 */
                seq_t S = (seq_t)u * ((seq_t)a + (seq_t)b) / 2;

                /* store if space permits */
                if (temp_len >= temp_cap) {
                    temp_cap *= 2;
                    seq_t *newp = (seq_t *)realloc(temp,
                                                  temp_cap * sizeof(seq_t));
                    if (!newp) { free(temp); return; }
                    temp = newp;
                }
                temp[temp_len++] = S;
            }
        }

        /* sort and deduplicate */
        qsort(temp, temp_len, sizeof(seq_t), cmp_seq);

        /* count unique values */
        size_t uniq_len = 0;
        seq_t *uniq = (seq_t *)malloc(temp_len * sizeof(seq_t));
        if (!uniq) { free(temp); return; }
        for (size_t i = 0; i < temp_len; ++i) {
            if (i == 0 || temp[i] != temp[i - 1]) {
                uniq[uniq_len++] = temp[i];
            }
        }
        free(temp);

        if (uniq_len >= (size_t)n) {
            cache = uniq;
            cache_len = uniq_len;
            break;          /* sufficient terms */
        }

        /* not enough terms – enlarge search space */
        free(uniq);
        max_uv *= 2;
        /* safety guard – should never hit this with n<=10000 */
        if (max_uv > 20000) { max_uv = 20000; }
    }
}

/* Required function signature */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1; n>=1 in practice */
    build_cache(n);
    return cache[n - 1];
}
