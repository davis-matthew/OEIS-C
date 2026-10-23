/*
 * A143404  Expansion of x^k/Product_{t=k..2k} (1-tx) for k=9.
 * https://oeis.org/A143404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = x^9/((1-9x)(1-10x)...(1-18x)) Recurrence (order 10):
 *            a(n)=135 a(n‑1) – 8160 a(n‑2)+290790 a(n‑3) – 6765213
 *            a(n‑4)+107358615 a(n‑5) – 1176812090 a(n‑6)+8797620060 a(n‑7) –
 *            42924478536 a(n‑8)+123418922400 a(n‑9) – 158789030400 a(n‑10)
 *            Initial values: a(0)=…=a(8)=0, a(9)=1
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation array */
static idx_t cache_len = 0;     /* number of entries currently stored */

seq_t a(idx_t n)
{
    /* initialise cache with base values (indices 0..9) on first call */
    if (cache == NULL) {
        cache_len = 10;                     /* 0..9 inclusive */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 9; ++i) cache[i] = 0;
        cache[9] = 1;                       /* a(9) = 1 */
    }

    /* extend cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();                  /* memory allocation failed */
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 9) {
                cache[i] = 0;                /* base zeros */
            } else if (i == 9) {
                cache[i] = 1;                /* a(9) = 1 */
            } else {
                seq_t pos = 0;
                pos += (seq_t)135       * cache[i-1];
                pos += (seq_t)290790    * cache[i-3];
                pos += (seq_t)107358615 * cache[i-5];
                pos += (seq_t)8797620060 * cache[i-7];
                pos += (seq_t)123418922400 * cache[i-9];

                seq_t neg = 0;
                neg += (seq_t)8160      * cache[i-2];
                neg += (seq_t)6765213   * cache[i-4];
                neg += (seq_t)1176812090 * cache[i-6];
                neg += (seq_t)42924478536 * cache[i-8];
                neg += (seq_t)158789030400 * cache[i-10];

                cache[i] = pos - neg;            /* pos ≥ neg for this sequence */
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
