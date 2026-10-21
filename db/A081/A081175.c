/*
 * A081175  Numbers of the form Sum_{i=1..k} i^j, j >= 1, k >= 1.
 * https://oeis.org/A081175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_limit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A081175: numbers that can be written as  sum_{i=1..k} i^j,
 *  with j >= 1, k >= 1.
 *
 *  This implementation builds all such sums up to a hard‑coded
 *  maximum value (5 000 000 000).  The count of distinct sums below
 *  that limit is roughly 1 00 000, so the routine is guaranteed to
 *  provide correct answers for all n ≤ 10 000 and is fast for
 *  typical values of n.  The set of sums is generated once,
 *  sorted, deduplicated and stored in a static array.  Subsequent
 *  calls simply index into that array.
 *
 *  The calculation avoids 64‑bit overflows by limiting intermediate
 *  results with the same upper bound used for the final sums.
 *
 *  The function does not rely on any special language features;
 *  it uses only standard C (<stdlib.h>, <stdint.h>, <stdbool.h>, <string.h>).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow_limit_uint64.h"

/* Comparison for qsort on seq_t values. */
static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* Returns the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache of all terms up to a hard‑coded maximum sum. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Hard‑coded upper bound sufficient for n up to about 100 000. */
        const seq_t MAX_SUM = 5000000000ULL;   /* 5 000 000 000 */

        size_t cap = 1 << 20;                 /* initial capacity ~1 048 576 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failed */
        size_t len = 0;

        /* Generate sums S(j,k) = sum_{i=1..k} i^j for all j>=1, k>=1. */
        for (uint64_t j = 1;; ++j) {
            /* If 2^j > MAX_SUM, then k>=2 gives sums > MAX_SUM;
               only k==1 contributes the value 1, which is already included,
               so we can stop. */
            if (pow_limit_uint64(2ULL, j, MAX_SUM) > MAX_SUM) break;

            seq_t sum = 0;
            for (uint64_t k = 1;; ++k) {
                seq_t p = pow_limit_uint64(k, j, MAX_SUM);
                if (p > MAX_SUM) break;                /* further powers too big */
                if (sum > MAX_SUM - p) break;          /* would overflow limit */
                sum += p;
                if (sum > MAX_SUM) break;

                /* Store the sum. */
                if (len == cap) {
                    cap <<= 1;                        /* double capacity */
                    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    if (!tmp) { free(cache); cache = NULL; return 0; }
                    cache = tmp;
                }
                cache[len++] = sum;
            }
        }

        /* Sort and remove duplicates. */
        qsort(cache, len, sizeof(seq_t), cmp_seq);
        /* Deduplicate in-place. */
        size_t unique_len = 0;
        if (len > 0) {
            cache[unique_len++] = cache[0];
            for (size_t i = 1; i < len; ++i) {
                if (cache[i] != cache[unique_len - 1]) {
                    cache[unique_len++] = cache[i];
                }
            }
        }
        cache_len = unique_len;
        initialized = true;
    }

    if (n == 0 || (seq_t)n > cache_len) return 0;   /* out of range */
    return cache[n - 1];
}
