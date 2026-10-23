/*
 * A015107  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-10.
 * https://oeis.org/A015107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n+1) = sum_{i=0..n} (-10)^i * a(i) * a(n-i) ,  a(0)=1  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0;          /* number of elements stored in cache   */
    static size_t cap = 0;          /* capacity of cache                     */

    /* initialize cache with a(0) */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = 1;
        len = 1;
    }

    /* extend cache up to index n */
    while (len <= n) {
        idx_t m = len - 1;          /* current largest computed index        */
        seq_t sum = 0;
        seq_t pow_q = 1;            /* (-10)^0 */

        for (idx_t i = 0; i <= m; ++i) {
            seq_t prod = cache[i] * cache[m - i];
            prod = prod * pow_q;
            sum += prod;
            pow_q *= (seq_t)-10;    /* multiply by q to get next power      */
        }

        /* grow cache if necessary */
        if (len + 1 > cap) {
            size_t new_cap = cap * 2;
            if (new_cap < len + 1) new_cap = len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache = tmp;
            cap   = new_cap;
        }

        cache[len++] = sum;        /* store a(m+1) */
    }

    return cache[n];
}
