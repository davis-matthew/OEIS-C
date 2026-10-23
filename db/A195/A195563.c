/*
 * A195563  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 1/4.
 * https://oeis.org/A195563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 65*a(n-3) + 65*a(n-6) - a(n-9) (n ≥ 10) a(1)=0, a(2)=7,
 *            a(3)=9, a(4)=16, a(5)=399, a(6)=657, a(7)=1040, a(8)=26391,
 *            a(9)=43289
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[10] = {
    0,          /* dummy for index 0 */
    0,          /* a(1) */
    7,          /* a(2) */
    9,          /* a(3) */
    16,         /* a(4) */
    399,        /* a(5) */
    657,        /* a(6) */
    1040,       /* a(7) */
    26391,      /* a(8) */
    43289       /* a(9) */
};

seq_t a(idx_t n)
{
    if (n <= 9)
        return _init[(size_t)n];

    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* indices valid: 0 .. cache_len-1 */

    if (!cache) {
        cache_len = 10;                   /* need indices 0..9 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = _init[i];
    }

    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (size_t i = cache_len; i < new_len; ++i) {
            if (i <= 9) {
                cache[i] = _init[i];
            } else {
                cache[i] = (seq_t)65 * cache[i-3]
                         + (seq_t)65 * cache[i-6]
                         - cache[i-9];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
