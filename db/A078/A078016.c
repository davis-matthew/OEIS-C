/*
 * A078016  Expansion of (1-x)/(1-x+x^2+x^3).
 * https://oeis.org/A078016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - a(n-2) - a(n-3) a(0)=1, a(1)=0, a(2)=-1
 * Range:     i8 n <= 18, i16 n <= 35, i32 n <= 73, i64 n <= 145,
 *            i128 n <= 291, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization buffer */
    static seq_t *buf = NULL;
    static size_t buf_size = 0;  /* allocated elements */
    static size_t computed = 0;  /* number of elements stored (first computed element index +1) */

    size_t N = (size_t)n;        /* index as a size_t -- safe for small n */

    /* initialise buffer if first call */
    if (buf == NULL) {
        buf_size = (N > 2) ? N + 1 : 3;   /* at least capacity for the first three terms */
        buf = (seq_t*)malloc(buf_size * sizeof(seq_t));
        if (buf == NULL) return (seq_t)0; /* allocation failed – return zero as a safe fallback */

        /* base cases */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)0;
        buf[2] = (seq_t)-1;
        computed = 3;            /* a[0] … a[2] have been written */
    }
    /* grow buffer if needed */
    if (N + 1 > buf_size) {
        size_t new_size = buf_size;
        while (new_size < N + 1) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* realloc failed – return zero */
        buf = tmp;
        buf_size = new_size;
    }

    /* if value already computed, return it */
    if (N < computed) {
        return buf[N];
    }

    /* compute up to N (inclusive) */
    for (size_t i = computed; i <= N; ++i) {
        if (i < 3) continue;  /* base cases already set */
        buf[i] = buf[i-1] - buf[i-2] - buf[i-3];
    }
    computed = N + 1;
    return buf[N];
}
