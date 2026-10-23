/*
 * A160209  Positive numbers y such that y^2 is of the form x^2+(x+937)^2 with
 *          integer x.
 * https://oeis.org/A160209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3)-a(n-6) for n>6; a(1)=673, a(2)=937, a(3)=1685,
 *            a(4)=2353, a(5)=4685, a(6)=9437
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* cache[1..max_index] valid */
    static idx_t  max_index = 0;

    /* initial values */
    static const seq_t init[6] = {673, 937, 1685, 2353, 4685, 9437};

    /* ensure cache is large enough */
    if (n > max_index) {
        size_t new_size = (size_t)(n + 1);          /* index 0 unused */
        if (!cache) {
            cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else if (new_size > (size_t)max_index + 1) {
            cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }
        /* fill missing entries */
        if (max_index < 6) {
            size_t start = (size_t)(max_index + 1);
            for (size_t i = start; i <= 6 && i <= (size_t)n; ++i) {
                cache[i] = init[i - 1];
            }
        }
        for (idx_t i = (idx_t)max_index + 1; i <= n; ++i) {
            if (i <= 6) continue;            /* already filled */
            cache[i] = (6 * cache[i - 3]) - cache[i - 6];
        }
        max_index = n;
    }
    return cache[n];
}
