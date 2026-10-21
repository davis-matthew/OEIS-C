/*
 * A050137  a(1)=2; a(n) = floor(a(n-1)/2) if this is not among
 *          0,a(1),...,a(n-1); otherwise a(n) = 2*n.
 * https://oeis.org/A050137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2; for n>1, a(n) = floor(a(n‑1)/2) if that value
   has not appeared before (or is zero), otherwise a(n) = 2*n */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of already computed values */
    static size_t cache_len = 0;         /* number of values stored */
    static size_t cache_cap = 0;         /* allocated capacity */

    /* the sequence is defined for n >= 1; n == 0 can return 0 */
    if (n == 0) return 0;

    /* initialisation on first call */
    if (cache == NULL) {
        const size_t INIT_CAP = 16;
        cache_cap = INIT_CAP;
        cache_len = 0;
        /* allocate initial storage */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        cache[0] = (seq_t)2;   /* a(1) = 2 */
        cache_len = 1;
    }

    /* grow cache if necessary up to the requested index */
    size_t target = (size_t)n;                 /* number of terms needed */
    while (cache_len < target) {
        /* enlarge storage if full */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
            for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
            delete[] cache;
#else
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute the next term */
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev >> 1;   /* floor(prev/2) */

        /* check whether cand is unused (or zero) */
        bool used = false;
        if (cand == 0) {
            used = true;
        } else {
            for (size_t i = 0; i < cache_len; ++i) {
                if (cache[i] == cand) {
                    used = true;
                    break;
                }
            }
        }

        seq_t val;
        if (!used) {
            val = cand;
        } else {
            /* 2 * n where n = cache_len + 1 (next index) */
            val = (seq_t)(2ULL * (uint64_t)(cache_len + 1));
        }

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n - 1];
}
