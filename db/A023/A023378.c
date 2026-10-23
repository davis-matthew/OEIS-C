/*
 * A023378  a(n+1) = a(n) converted to base 10 from base 4 (written in base
 *          10); a(1)=4.
 * https://oeis.org/A023378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = a(n) converted to base 10 from base 4 (written in base
 *            10); a(1)=4
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t next_val(seq_t x)
{
    seq_t result = 0;
    seq_t pow10   = 1;
    while (x != 0) {
        seq_t digit = x % 4;
        result += digit * pow10;
        pow10  *= 10;
        x     /= 4;
    }
    return result;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static idx_t cached_len  = 0;

    if (n < 1)
        return 0;                /* sequence offset is 1 */

    if (n > cached_len) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp)
            return 0;              /* allocation failed */
        cache = tmp;

        if (cached_len == 0) {
            cache[0] = 4;          /* a(1) */
            cached_len = 1;
        }

        for (idx_t i = cached_len; i < n; ++i)
            cache[i] = next_val(cache[i - 1]);   /* a(i+1) from a(i) */
        cached_len = n;
    }

    return cache[n - 1];
}
