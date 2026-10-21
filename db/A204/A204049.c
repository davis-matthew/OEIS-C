/*
 * A204049  Number of (n+2)X3 0..3 arrays with every 3X3 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A204049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 31*a(n-2) + 69*a(n-3) - 176*a(n-4) - 8*a(n-5)
 *            - 116*a(n-6) - 132*a(n-7) + 240*a(n-8) + 144*a(n-9) (empirical).
 *            Initial values: a(1)=1379, a(2)=10447, a(3)=80391, a(4)=622031,
 *            a(5)=4823759, a(6)=37451727, a(7)=290942303, a(8)=2260735279,
 *            a(9)=17568741535
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;

    /* first nine terms */
    static const seq_t init[9] = {
        (seq_t)1379,
        (seq_t)10447,
        (seq_t)80391,
        (seq_t)622031,
        (seq_t)4823759,
        (seq_t)37451727,
        (seq_t)290942303,
        (seq_t)2260735279,
        (seq_t)17568741535
    };

    /* lazy initialization of cache */
    if (cache == NULL) {
        idx_t need = n + 1;
        if (need < 10) need = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * need);
        cache_size = need;
        for (idx_t i = 1; i <= 9; ++i) {
            cache[i] = init[i - 1];
        }
        computed = 9;
    }

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    if (n <= computed) return cache[n];

    /* compute remaining terms using recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val =   cache[i - 1] * (seq_t)11
                  - cache[i - 2] * (seq_t)31
                  + cache[i - 3] * (seq_t)69
                  - cache[i - 4] * (seq_t)176
                  - cache[i - 5] * (seq_t)8
                  - cache[i - 6] * (seq_t)116
                  - cache[i - 7] * (seq_t)132
                  + cache[i - 8] * (seq_t)240
                  + cache[i - 9] * (seq_t)144;
        cache[i] = val;
    }

    computed = n;
    return cache[n];
}
