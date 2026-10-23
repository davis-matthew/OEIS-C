/*
 * A041502  Numerators of continued fraction convergents to sqrt(268).
 * https://oeis.org/A041502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the n-th convergent to sqrt(268). The
 *            partial quotients are periodic after the initial term 16: a0 =
 *            16, a1..a20 repeat with period 20: [2, 1, 2, 3, 3, 1, 3, 1, 10,
 *            8, 10, 1, 3, 1, 3, 3, 2, 1, 2, 32]. Recurrence for the
 *            numerators p_n: p_0 = 16 p_1 = 33 (since a1 = 2) p_n...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed numerators */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored so far */
    static size_t capacity = 0;    /* allocated capacity */

    /* periodic partial quotients (after a0) */
    static const unsigned int period_a[20] = {
        2, 1, 2, 3, 3, 1, 3, 1, 10, 8,
        10, 1, 3, 1, 3, 3, 2, 1, 2, 32
    };
    static const unsigned int period_len = 20;
    static const unsigned int a0_val = 16;

    /* ensure cache is large enough */
    if (cache == NULL) {
        capacity = 16;                  /* initial capacity */
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (!cache) return 0;           /* allocation failed, return 0 */
        /* compute p0 */
        cache[0] = (seq_t)a0_val;
        cache_len = 1;
    }

    /* grow capacity if needed */
    while (capacity <= (size_t)n) {
        size_t new_cap = capacity * 2;
        if (new_cap < capacity + 1) new_cap = capacity + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute values up to n if not yet computed */
    while (cache_len <= (size_t)n) {
        size_t idx = cache_len;          /* current index i */
        unsigned int a_k;
        if (idx == 0) {
            a_k = a0_val;
        } else {
            a_k = period_a[(idx - 1) % period_len];
        }

        seq_t prev = cache[idx - 1];
        seq_t prev_prev;
        if (idx - 1 >= 1)
            prev_prev = cache[idx - 2];
        else
            prev_prev = (seq_t)1;        /* p_{-1} = 1 for the first step */

        cache[idx] = (seq_t)a_k * prev + prev_prev;
        cache_len++;
    }

    return cache[(size_t)n];
}
