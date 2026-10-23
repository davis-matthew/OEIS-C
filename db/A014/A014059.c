/*
 * A014059  Inverse of 50th cyclotomic polynomial.
 * https://oeis.org/A014059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1. For n > 0, a(n) = a(n‑5) – a(n‑10) + a(n‑15) –
 *            a(n‑20), with a(k) = 0 for k < 0. This recurrence generates a
 *            periodic sequence of period 50
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N >= cache_len) {
        size_t new_len = N + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) abort();

        if (cache_len == 0) {
            /* first call: seed a(0)=1 and compute up to N */
            new_cache[0] = (seq_t)1;
            for (size_t i = 1; i <= N; ++i) {
                seq_t val = (seq_t)0;
                if (i >= 5)  val += new_cache[i-5];
                if (i >= 10) val -= new_cache[i-10];
                if (i >= 15) val += new_cache[i-15];
                if (i >= 20) val -= new_cache[i-20];
                new_cache[i] = val;
            }
        } else {
            /* extend existing cache */
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            for (size_t i = cache_len; i <= N; ++i) {
                seq_t val = (seq_t)0;
                if (i >= 5)  val += new_cache[i-5];
                if (i >= 10) val -= new_cache[i-10];
                if (i >= 15) val += new_cache[i-15];
                if (i >= 20) val -= new_cache[i-20];
                new_cache[i] = val;
            }
        }

        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    return cache[N];
}
