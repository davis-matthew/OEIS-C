/*
 * A077989  Expansion of 1/(1+2*x+x^2-2*x^3).
 * https://oeis.org/A077989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - a(n-2) + 2*a(n-3) (n ≥ 3) a(0)=1, a(1)=-2,
 *            a(2)=3 Corresponds to the expansion of 1/(1+2*x+x^2-2*x^3)
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 42, i64 n <= 82, i128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* size of allocated array */
    static idx_t last_filled = 0;    /* highest index whose value is known */

    /* --- initialise on first call ------------------------------------ */
    if (cache == NULL) {
        cache_len = 3;              /* we store a0,a1,a2 at least */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) abort(); /* fatal */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)3;
        last_filled = 2;
    }

    /* --- expand cache if needed ------------------------------------- */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = cache_len;
        while (new_len < (size_t)(n + 1))
            new_len *= 2;          /* geometric growth */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* --- compute missing terms ------------------------------------- */
    if (n > last_filled) {
        for (idx_t i = last_filled + 1; i <= n; ++i) {
            seq_t a_nm1 = cache[i - 1];
            seq_t a_nm2 = cache[i - 2];
            seq_t a_nm3 = cache[i - 3];
            cache[i] = -(seq_t)2 * a_nm1 - a_nm2 + (seq_t)2 * a_nm3;
        }
        last_filled = n;
    }

    return cache[n];
}
