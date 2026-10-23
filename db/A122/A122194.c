/*
 * A122194  Numbers that are the sum of exactly two sets of Fibonacci numbers.
 * https://oeis.org/A122194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) + 1 (n>4) a(1)=3, a(2)=5, a(3)=6, a(4)=9
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 87, u64 n <= 179,
 *            u128 n <= 363, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    static seq_t *cache = NULL;
    static size_t computed = 0;    /* highest index already computed   */
    static size_t capacity = 0;    /* allocated size (>= computed+1)  */

    size_t idx = (size_t)n;
    /* Ensure allocated capacity covers index n */
    if (capacity < idx + 1) {
        size_t new_cap = idx + 1;
        seq_t *newbuf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newbuf) exit(1);          /* allocation failure */
        /* zero newly allocated entries */
        for (size_t i = capacity; i < new_cap; ++i)
            newbuf[i] = 0;
        cache = newbuf;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    if (computed < idx) {
        for (size_t i = computed + 1; i <= idx; ++i) {
            if (i == 1)         cache[i] = (seq_t)3;
            else if (i == 2)    cache[i] = (seq_t)5;
            else if (i == 3)    cache[i] = (seq_t)6;
            else if (i == 4)    cache[i] = (seq_t)9;
            else                cache[i] = cache[i-2] + cache[i-4] + (seq_t)1;
        }
        computed = idx;
    }

    return cache[idx];
}
