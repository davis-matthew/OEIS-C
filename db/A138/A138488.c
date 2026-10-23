/*
 * A138488  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 4.
 * https://oeis.org/A138488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(0)=4. For n>0, let T = a(n-1). Scan T from rightmost digit to left.
   For each digit d that is encountered for the first time, output the total
   count of d in T, followed by d. The concatenation of these output digits is a(n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Compute a(n) for the described recurrence, with memoisation. */
seq_t a(idx_t n) {
    if ((size_t)n < cache_len)
        return cache[n];

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache)
        abort();

    /* copy previously computed terms */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            new_cache[0] = 4;   /* initial term */
            continue;
        }

        seq_t prev = new_cache[i - 1];
        unsigned int freq[10];
        digit_frequencies(prev, freq);

        bool seen[10] = { false };
        uint8_t digits[200];      /* buffer for the new term's digits */
        size_t dlen = 0;

        seq_t temp = prev;
        while (temp > 0) {
            unsigned int d = (unsigned int)(temp % (seq_t)10);
            if (!seen[d]) {
                seen[d] = true;
                unsigned int cnt = freq[d];

                /* append decimal digits of cnt (most‑significant first) */
                unsigned int t = cnt;
                uint8_t tmp[10];
                size_t clen = 0;
                if (t == 0) {
                    tmp[clen++] = 0;
                } else {
                    while (t > 0) {
                        tmp[clen++] = t % 10;
                        t /= 10;
                    }
                    for (size_t k = 0; k < clen / 2; ++k) {
                        uint8_t swap = tmp[k];
                        tmp[k] = tmp[clen - 1 - k];
                        tmp[clen - 1 - k] = swap;
                    }
                }
                for (size_t k = 0; k < clen; ++k)
                    digits[dlen++] = tmp[k];

                digits[dlen++] = (uint8_t)d;
            }
            temp /= 10;
        }

        /* convert digit buffer back to seq_t */
        seq_t next = 0;
        for (size_t k = 0; k < dlen; ++k)
            next = next * (seq_t)10 + (seq_t)digits[k];

        new_cache[i] = next;
    }

    free(cache);
    cache = new_cache;
    cache_len = new_len;
    return cache[n];
}
