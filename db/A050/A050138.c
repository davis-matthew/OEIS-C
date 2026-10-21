/*
 * A050138  a(1)=2, a(2)=6. For n >= 3, a(n) = floor(a(n-1)/2) if this is not
 *          among 0,a(1),...,a(n-1); otherwise a(n) = 3*n.
 * https://oeis.org/A050138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1)=2, a(2)=6; for n>=3, a(n) = floor(a(n‑1)/2) if that value
 *            has not appeared before (including 0), otherwise a(n) = 3*n
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for computed values; index 0 unused to mimic 1‑based sequence */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* allocated capacity */
    static size_t len = 0;       /* number of elements stored (including dummy) */

    /* initialise on first call */
    if (len == 0) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;    /* dummy value; 0 is considered used */
        len = 1;
    }

    /* grow cache until it can hold index n */
    while (len <= (size_t)n) {
        size_t i = len;           /* compute a(i) */
        seq_t val;

        if (i == 1) {
            val = (seq_t)2;
        } else if (i == 2) {
            val = (seq_t)6;
        } else {
            seq_t cand = cache[i - 1] >> 1;   /* floor(a(i-1)/2) */

            /* check if cand has appeared before (including 0) */
            bool used = false;
            for (size_t j = 0; j < i - 1; ++j) {
                if (cache[j] == cand) {
                    used = true;
                    break;
                }
            }
            if (!used)
                val = cand;
            else
                val = (seq_t)i * (seq_t)3;
        }

        /* ensure capacity */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            while (new_cap <= len) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        cache[len++] = val;
    }

    return cache[n];
}
