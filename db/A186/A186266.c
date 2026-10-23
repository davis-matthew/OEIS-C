/*
 * A186266  Expansion of 2F1( 1/2, 3/2; 4; 16*x ).
 * https://oeis.org/A186266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, and for n >= 1
   a(n) = 4*(2*n-1)*(2*n+1)*a(n-1) / (n*(n+3))        */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *a_cache = NULL;   /* cache of computed terms */
static size_t a_cached = 0;     /* highest index cached + 1 */

/* ensure that a_cache[n] is available for all n ≤ max */
static void a_ensure(idx_t max)
{
    if (a_cached > max) return;

    /* allocate (max+1) entries */
    size_t new_sz = (size_t)max + 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_sz * sizeof(seq_t));
    if (!new_cache) exit(1);  /* allocation failure; abort */

    for (idx_t i = a_cached; i <= max; ++i) {
        if (i == 0) {
            new_cache[0] = (seq_t)1;      /* a(0) = 1 */
        } else {
            seq_t prev = new_cache[i-1];

            /* numerator factors */
            seq_t num0 = (seq_t)4;
            seq_t num1 = (seq_t)(2*i - 1);
            seq_t num2 = (seq_t)(2*i + 1);

            /* denominator factors */
            seq_t den0 = (seq_t)i;
            seq_t den1 = (seq_t)(i + 3);

            /* reduce numerators with denominators */
            seq_t g;
            g = gcd(num0, den0);  num0 /= g;  den0 /= g;
            g = gcd(num0, den1);  num0 /= g;  den1 /= g;
            g = gcd(num1, den0);  num1 /= g;  den0 /= g;
            g = gcd(num1, den1);  num1 /= g;  den1 /= g;
            g = gcd(num2, den0);  num2 /= g;  den0 /= g;
            g = gcd(num2, den1);  num2 /= g;  den1 /= g;

            /* cancel remaining denominators with the previous term */
            seq_t res = prev;
            g = gcd(res, den0);   res /= g;  den0 /= g;
            g = gcd(res, den1);   res /= g;  den1 /= g;

            /* after reductions, den0 and den1 should be 1 */
            /* multiply by the reduced numerators */
            res *= num0;
            res *= num1;
            res *= num2;

            new_cache[i] = res;
        }
    }

    a_cache = new_cache;
    a_cached = new_sz;
}

seq_t a(idx_t n)
{
    a_ensure(n);
    return a_cache[n];
}
