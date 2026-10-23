/*
 * A119516  Expansion of x*(7*x^3+7*x^2+9*x+1)/(5*x^4+10*x^2+1).
 * https://oeis.org/A119516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=9, a(3)=-3, a(4)=-83, recurrence: a(n) =
 *            b(n) – 10*a(n-2) – 5*a(n-4), where b(n)=1 (n=1), 9 (n=2), 7 (n=3
 *            or 4), else 0
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 19, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t b = 0;
            if (i == 1)       b = (seq_t)1;
            else if (i == 2)  b = (seq_t)9;
            else if (i == 3)  b = (seq_t)7;
            else if (i == 4)  b = (seq_t)7;

            seq_t val = b;
            if (i >= 2) val -= (seq_t)10 * cache[i - 2];
            if (i >= 4) val -= (seq_t)5 * cache[i - 4];

            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
