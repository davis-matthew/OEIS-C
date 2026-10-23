/*
 * A143604  Polynomial expansion sequence : p(x)=1 + x^2 - x^3 - x^5 - x^7 +
 *          x^8 + x^10.
 * https://oeis.org/A143604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-2)+a(n-3)+a(n-5)+a(n-7)-a(n-8)-a(n-10); Base:
 *            a(0)=1, a(1)=0 This implements the linear recurrence derived
 *            from 1 + x^2 - x^3 - x^5 - x^7 + x^8 + x^10 giving a(n)=coeff.
 *            of x^n in 1/(1 + x^2 - x^3 - x^5 - x^7 + x^8 + x^10)
 * Range:     i8 n <= 43, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The types seq_t and idx_t are supplied by the build system. */
extern seq_t a(idx_t n);

static seq_t   *cache   = NULL;   /* dynamic array of computed terms */
static size_t  cap      = 0;     /* current capacity of cache      */
static idx_t   last     = 0;     /* largest index that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;

    /* make sure the cache can hold indices up to n */
    if (n >= (idx_t)cap) {
        size_t new_cap = (size_t)n + 1;
        if (cap == 0) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cap = new_cap;
        /* initialize base values if they have been destroyed */
        cache[0] = 1;
        cache[1] = 0;
    }

    /* compute all terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 2) val -= cache[(size_t)(i - 2)];
        if (i >= 3) val += cache[(size_t)(i - 3)];
        if (i >= 5) val += cache[(size_t)(i - 5)];
        if (i >= 7) val += cache[(size_t)(i - 7)];
        if (i >= 8) val -= cache[(size_t)(i - 8)];
        if (i >= 10) val -= cache[(size_t)(i - 10)];
        cache[(size_t)i] = val;
    }
    last = n > last ? n : last;
    return cache[(size_t)n];
}
