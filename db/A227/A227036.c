/*
 * A227036  Expansion of 2*(1+x^2)/((1-x)*(1-x-2*x^3)).
 * https://oeis.org/A227036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 2*(1+x^2)/((1-x)*(1-x-2*x^3))
 *            Recurrence (valid for all n ≥ 0, with a(k)=0 for k<0): a(n) =
 *            2*a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*I(n==0 || n==2)
 *            where I(condition) is 1 if condition holds, otherwise 0
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 81, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cached = 0;

seq_t a(idx_t n) {
    /* Allocate and initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)2;                 /* a(0) */
        cached = 0;
    }

    /* Extend cache up to n if necessary */
    if (n > cached) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;    /* allocation failure */
        cache = tmp;

        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i == 0) {      /* already initialised */
                cache[i] = (seq_t)2;
                continue;
            }
            seq_t term = (seq_t)2 * cache[i - 1];
            if (i >= 2) term -= cache[i - 2];
            if (i >= 3) term += (seq_t)2 * cache[i - 3];
            if (i >= 4) term -= (seq_t)2 * cache[i - 4];
            if (i == 0 || i == 2) term += (seq_t)2;   /* 2*b(i) */
            cache[i] = term;
        }
        cached = n;
    }

    return cache[n];
}
