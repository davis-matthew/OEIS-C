/*
 * A129991  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+241)^2 = y^2.
 * https://oeis.org/A129991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 482 for n > 6 Initial values: a(1)=0,
 *            a(2)=23, a(3)=620, a(4)=723, a(5)=840, a(6)=4223
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 68, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed = 0;

static const seq_t INIT[6] = {
    (seq_t)0ULL,  /* a(1) */
    (seq_t)23ULL, /* a(2) */
    (seq_t)620ULL,/* a(3) */
    (seq_t)723ULL,/* a(4) */
    (seq_t)840ULL,/* a(5) */
    (seq_t)4223ULL/* a(6) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (n > cache_len) {
        size_t new_len = cache_len ? cache_len : 6;
        while (new_len < (size_t)n)
            new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_len = new_len;
    }

    if (computed < 6) {
        idx_t upto = n < 6 ? n : 6;
        for (idx_t i = computed; i < upto; ++i)
            cache[i] = INIT[i];
        computed = upto;
    }

    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i - 1] = INIT[i - 1];
        } else {
            cache[i - 1] = cache[i - 4] * (seq_t)6 - cache[i - 7] + (seq_t)482;
        }
    }

    computed = n;
    return cache[n - 1];
}
