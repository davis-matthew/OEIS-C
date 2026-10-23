/*
 * A041564  Numerators of continued fraction convergents to sqrt(300).
 * https://oeis.org/A041564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 2702*a(n-4) - a(n-8) Initial values: a(0)=17, a(1)=52,
 *            a(2)=433, a(3)=1351, a(4)=46367, a(5)=140452, a(6)=1169983,
 *            a(7)=3650401
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[8] = {
    (seq_t)17,
    (seq_t)52,
    (seq_t)433,
    (seq_t)1351,
    (seq_t)46367,
    (seq_t)140452,
    (seq_t)1169983,
    (seq_t)3650401
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;          /* highest index filled in cache + 1 */

    if (n < cached_len)   /* already computed */
        return cache[n];

    /* Grow cache to accommodate index n */
    size_t new_size = (size_t)(n + 1) * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_size);
    if (tmp == NULL) {
        abort();        /* allocation failure */
    }
    cache = tmp;

    /* Compute all new entries up to n */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            /* a(i) = 2702*a(i-4) - a(i-8) */
            cache[i] = cache[i - 4] * (seq_t)2702 - cache[i - 8];
        }
    }
    cached_len = n + 1;
    return cache[n];
}
