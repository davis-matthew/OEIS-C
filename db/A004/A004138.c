/*
 * A004138  From a counter moving problem.
 * https://oeis.org/A004138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + 2*a(n-4) + 2 for n >= 5, a(1)
 *            = 1, a(2) = 2, a(3) = 3, a(4) = 8
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[1..computed] are valid */
static idx_t computed = 0;
static idx_t capacity = 0;    /* allocated entries: indices 0..capacity-1 */

seq_t a(idx_t n) {
    if (n < 1) return 0;

    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (idx_t i = 0; i < capacity; i++) cache[i] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 8;
        computed = 4;
    }

    if (n > computed) {
        if (n >= capacity) {
            idx_t new_cap = capacity;
            while (new_cap <= n) new_cap *= 2;
            seq_t *nc = (seq_t *)malloc(new_cap * sizeof(seq_t));
            for (idx_t i = 0; i < new_cap; i++) nc[i] = 0;
            for (idx_t i = 0; i <= computed; i++) nc[i] = cache[i];
            free(cache);
            cache = nc;
            capacity = new_cap;
        }
        for (idx_t i = computed + 1; i <= n; i++) {
            seq_t v = cache[i-1] + cache[i-2];
            v += 2 * cache[i-4];
            v += 2;
            v -= cache[i-3];
            cache[i] = v;
        }
        computed = n;
    }

    return cache[n];
}
