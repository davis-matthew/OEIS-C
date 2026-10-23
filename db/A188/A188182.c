/*
 * A188182  Number of strictly increasing arrangements of 4 numbers in
 *          -(n+2)..(n+2) with sum zero.
 * https://oeis.org/A188182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) (a(1)=5, a(2)=12, a(3)=24, a(4)=43, a(5)=69, a(6)=104)
 * Range:     u8 n <= 8, u16 n <= 64, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;
    static size_t capacity = 0;

    /* ensure sufficient storage */
    size_t need_cap = (n > 6 ? n : 6) + 1;          /* indices 0..n (or 0..6) */
    if (need_cap > capacity) {
        capacity = need_cap;
        cache = (seq_t*)realloc(cache, sizeof(seq_t)*capacity);
    }

    /* initialise base values */
    if (max_computed < 6) {
        if (max_computed < 1) cache[1] = 5;
        if (max_computed < 2) cache[2] = 12;
        if (max_computed < 3) cache[3] = 24;
        if (max_computed < 4) cache[4] = 43;
        if (max_computed < 5) cache[5] = 69;
        if (max_computed < 6) cache[6] = 104;
        max_computed = 6;
    }

    /* compute further terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t s1 = 3 * cache[i - 1];
        s1 += 2 * cache[i - 3];
        s1 += 3 * cache[i - 5];

        seq_t s2 = 3 * cache[i - 2];
        s2 += 3 * cache[i - 4];
        s2 += cache[i - 6];

        cache[i] = s1 - s2;
    }
    if (n > max_computed) max_computed = n;

    return cache[n];
}
