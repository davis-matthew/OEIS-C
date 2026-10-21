/*
 * A064481  Numbers which are divisible by the sum of their base-5 digits.
 * https://oeis.org/A064481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer that is divisible by the sum of its
 * base‑5 digits (i.e., the n‑th 5‑Harshad number).  The function
 * uses incremental search and caches previously computed terms so
 * that subsequent calls are fast.  The implementation works for
 * any seq_t/idx_t types provided by the build. */
#include <stdlib.h>
#include "../utils/digit_sum_base5.h"

static seq_t *cache = NULL;          /* array of cached terms          */
static size_t cache_len = 0;         /* number of terms currently cached */
static size_t cache_cap = 0;         /* capacity of the cache array    */
static seq_t next_candidate = 1;     /* next integer to test           */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1, but n=0 yields 0 for safety */

    /* ensure we have at least n terms cached */
    while (cache_len < n) {
        seq_t cand = next_candidate;

        seq_t s = digit_sum_base5(cand);
        if (s != 0 && cand % s == 0) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;                 /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }

    return cache[n - 1];           /* remember offset 1 */
}
