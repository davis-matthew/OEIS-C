/*
 * A033851  Numbers whose prime factors are 5 and 7.
 * https://oeis.org/A033851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35 * A003595(n), where A003595 lists the numbers * 5^i *
 *            7^j for i, j ≥ 0 in ascending order. The sequence thus *
 *            contains the sorted list of numbers whose prime factors are only
 *            * 5 and 7, multiplied by 35
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 66, u64 n <= 290,
 *            u128 n <= 1206, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"   /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h" /* seq_t *seq_realloc(seq_t*, size_t, size_t) */

/* static cache for the A003595 values */
static seq_t *seq = NULL;     /* pointer to cached values */
static size_t cache_len = 0;  /* number of stored elements */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t ptr5 = 0, ptr7 = 0;  /* next indices for *5 and *7 */

/* Return the nth element (offset 1) of the OEIS sequence A033851. */
seq_t a(idx_t n)
{
    if (n == 0)            /* safety: offset starts at 1 */
        return 0;

    /* initialise cache on first call */
    if (!seq) {
        cache_cap = 16;
        seq = seq_alloc(cache_cap);      /* allocate initial array */
        if (!seq) exit(EXIT_FAILURE);   /* allocation failure */
        seq[0] = (seq_t)1;              /* first term of A003595 */
        cache_len = 1;
        ptr5 = ptr7 = 0;
    }

    /* grow cache until it contains the n‑th element of A003595 */
    if ((size_t)cache_len < (size_t)n) {
        size_t target = (size_t)n;

        /* increase capacity exponentially */
        if (target > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < target) new_cap *= 2;
            seq_t *tmp = seq_realloc(seq, cache_cap, new_cap);
            if (!tmp) exit(EXIT_FAILURE);
            seq = tmp;
            cache_cap = new_cap;
        }

        /* generate the remaining terms up to target */
        for (size_t i = cache_len; i < target; ++i) {
            seq_t cand5 = (seq_t)5 * seq[ptr5];
            seq_t cand7 = (seq_t)7 * seq[ptr7];
            seq_t cand  = (cand5 <= cand7) ? cand5 : cand7;
            seq[i] = cand;
            if (cand == cand5) ++ptr5;
            if (cand == cand7) ++ptr7;
        }
        cache_len = target;
    }

    /* a(n) = 35 × A003595(n) = 35 × seq[n‑1] */
    return (seq_t)35 * seq[(size_t)n - 1];
}
