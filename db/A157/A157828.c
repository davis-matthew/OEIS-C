/*
 * A157828  Expansion of 1/q(x) where q(x)=x^12*p(1/x) and p(x)=24 + 440*x^3 +
 *          264*x^6 + x^12.
 * https://oeis.org/A157828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 a(n) = -264*a(n-6) - 440*a(n-9) - 24*a(n-12) for n>0
 *            (assume a(k)=0 for k<0)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 47, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last = (size_t)-1;

    if (n == 0)
        return (seq_t)1;

    size_t idx = (size_t)n;
    if (idx >= cap) {
        size_t new_cap = cap ? (cap << 1) : 1;
        while (new_cap <= idx)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;              /* new entries are zero */
        cache = tmp;
        cap = new_cap;
    }

    if (last == (size_t)-1) {
        cache[0] = (seq_t)1;
        last = 0;
    }

    for (size_t i = last + 1; i <= idx; ++i) {
        seq_t val = (seq_t)0;
        if (i >= 6) val += (seq_t)(-264) * cache[i - 6];
        if (i >= 9) val += (seq_t)(-440) * cache[i - 9];
        if (i >= 12) val += (seq_t)(-24) * cache[i - 12];
        cache[i] = val;
    }
    last = idx;
    return cache[idx];
}
