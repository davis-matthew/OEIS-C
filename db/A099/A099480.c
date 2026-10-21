/*
 * A099480  Count from 1, repeating 2*n five times.
 * https://oeis.org/A099480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n‑1) - 2*a(n‑2) + 2*a(n‑3) - 2*a(n‑4) + 2*a(n‑5) -
 *            a(n‑6), with a(0) = 1, a(1) = a(2) = a(3) = a(4) = a(5) = 2
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoised values           */
    static size_t capacity = 0;     /* allocated array size     */
    static idx_t computed = 0;      /* number of values computed*/

    /* initialise on first call */
    if (cache == NULL) {
        capacity = 16;                                     /* a modest start */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;                /* allocation fail */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)2;
        computed = 6;                                      /* first 6 terms filled */
    }

    /* fast return if already known */
    if ((size_t)n < computed) {
        return cache[(size_t)n];
    }

    /* grow the array if we need more space */
    if ((size_t)n >= capacity) {
        size_t new_capacity = capacity;
        if (new_capacity == 0) new_capacity = 1;
        while (new_capacity <= (size_t)n) {
            new_capacity <<= 1;                           /* double until fit   */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;                /* allocation fail */
        cache = tmp;
        capacity = new_capacity;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t v;
        if (i < 6) {                                     /* should not occur */
            v = (i == 0 ? (seq_t)1 : (seq_t)2);
        } else {
            v = (seq_t)2 * cache[(size_t)i-1]
              - (seq_t)2 * cache[(size_t)i-2]
              + (seq_t)2 * cache[(size_t)i-3]
              - (seq_t)2 * cache[(size_t)i-4]
              + (seq_t)2 * cache[(size_t)i-5]
              - cache[(size_t)i-6];
        }
        cache[(size_t)i] = v;
    }
    computed = n + 1;                                    /* all up to n are ready */

    return cache[(size_t)n];
}
