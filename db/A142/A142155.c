/*
 * A142155  Expansion of x/( 1+x-x^2-x^4-x^5-x^6-x^7+x^9+x^10 ).
 * https://oeis.org/A142155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=1, a(2)=-1, a(3)=2, a(4)=-3, a(5)=6, a(6)=-9, …. For n≥2
 *            the sequence satisfies the linear recurrence a(n) = -a(n-1) +
 *            a(n-2) + a(n-4) + a(n-5) + a(n-6) + a(n-7) - a(n-9) - a(n-10)
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 42, i64 n <= 83, i128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf   = NULL;
    static size_t buf_cap = 0;
    static size_t computed_len = 0;   /* number of populated entries (max index+1) */

    /* We need room for indices 0..n inclusive. */
    size_t need = (size_t)n + 1;
    if (need > buf_cap) {
        size_t new_cap = buf_cap ? buf_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        buf = tmp;
        for (size_t i = buf_cap; i < new_cap; ++i) buf[i] = 0;
        buf_cap = new_cap;
    }

    /* initialise a[0] and a[1] if not yet done */
    if (computed_len < 2) {
        buf[0] = 0;
        if (need > 1) buf[1] = 1;
        computed_len = 2;
    }

    /* compute missing values up to n */
    if (computed_len <= n) {
        for (size_t i = computed_len; i <= (size_t)n; ++i) {
            seq_t val = 0;
            val -= buf[i-1];                       /* -a(i-1) */
            if (i >= 2) val += buf[i-2];            /* +a(i-2) */
            if (i >= 4) val += buf[i-4];            /* +a(i-4) */
            if (i >= 5) val += buf[i-5];            /* +a(i-5) */
            if (i >= 6) val += buf[i-6];            /* +a(i-6) */
            if (i >= 7) val += buf[i-7];            /* +a(i-7) */
            if (i >= 9) val -= buf[i-9];            /* -a(i-9) */
            if (i >= 10) val -= buf[i-10];          /* -a(i-10) */
            buf[i] = val;
        }
        computed_len = (size_t)n + 1;
    }

    return buf[n];
}
