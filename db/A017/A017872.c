/*
 * A017872  Expansion of 1/(1-x^8-x^9-x^10-x^11-x^12-x^13-x^14).
 * https://oeis.org/A017872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 73, u32 n <= 134, u64 n <= 255,
 *            u128 n <= 497, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-8) + a(n-9) + a(n-10) + a(n-11) + a(n-12) + a(n-13) + a(n-14) for n>13
 * Initial values: a(0) = 1, a(1..7) = 0, a(8..14) = 1
 */

#include <stdlib.h>          /* malloc, realloc */

static seq_t *cache = NULL;       /* memoisation buffer */
static idx_t  cache_size = 0;     /* allocated size (max index + 1) */
static idx_t  computed = 0;       /* number of entries already computed, i.e. last computed index + 1 */

seq_t a(idx_t n)
{
    /* resize buffer if necessary */
    if (n >= cache_size) {
        cache_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
        cache = tmp;
    }

    /* compute missing values up to n */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i < 8) {
                cache[i] = (seq_t)0;
            } else if (i <= 14) {
                cache[i] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                sum += cache[i - 8];
                sum += cache[i - 9];
                sum += cache[i -10];
                sum += cache[i -11];
                sum += cache[i -12];
                sum += cache[i -13];
                sum += cache[i -14];
                cache[i] = sum;
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
