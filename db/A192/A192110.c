/*
 * A192110  Monotonic ordering of nonnegative differences 2^i - 3^j, for 40 >=
 *          i >= 0, j >= 0.
 * https://oeis.org/A192110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 91, u32 n <= 347, u64 n <= 534,
 *            u128 n <= 534, big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192110: Monotonic ordering of non‑negative differences 2^i – 3^j
   for 0 ≤ i ≤ 40 and all j ≥ 0.
   The sequence consists of all distinct values of 2^i – 3^j that are
   ≥ 0, sorted in increasing order.  a(n) returns the n‑th term
   (1‑indexed).  The implementation builds the entire list once
   on first call, then indexes into it. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/compare_seq_t.h"

static seq_t *cache = NULL;   /* unique sorted differences */
static size_t cache_len = 0;  /* number of stored values */

/* Return the n‑th term of A192110 (1‑indexed) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined below the offset */

    /* Build the cache only once */
    if (cache_len == 0) {
        size_t capacity = 1024;          /* initial space for differences */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        size_t len = 0;

        /* Max value that can fit in seq_t */
        seq_t max_seq = (seq_t)~(seq_t)0;
        idx_t max_seq_val = (idx_t)max_seq;

        /* Enumerate all 2^i – 3^j with 0 ≤ i ≤ 40 and non‑negative differences */
        for (idx_t i = 0; i <= 40; ++i) {
            idx_t pow2 = ((idx_t)1) << i;   /* 2^i */

            idx_t pow3 = 1;                 /* 3^j */
            for (/*j from 0*/ ; pow3 <= pow2; /* j incremented inside loop */) {
                idx_t diff = pow2 - pow3;
                if (diff <= max_seq_val) {
                    /* Grow the array if necessary */
                    if (len == capacity) {
                        capacity *= 2;
                        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                    }
                    cache[len++] = (seq_t)diff;
                }
                /* Next power of 3; stop if multiplication would overflow 64‑bit */
                if (pow3 > SIZE_MAX / 3)
                    break;
                pow3 *= 3;
            }
        }

        /* Sort values */
        qsort(cache, len, sizeof(seq_t), compare_seq_t);

        /* Remove duplicates in place */
        size_t uniq = 0;
        for (size_t i = 0; i < len; ++i) {
            if (uniq == 0 || cache[i] != cache[uniq - 1]) {
                cache[uniq++] = cache[i];
            }
        }
        cache_len = uniq;
    }

    if ((size_t)(n - 1) < cache_len)
        return cache[n - 1];
    else
        return 0;   /* n is out of range */
}
