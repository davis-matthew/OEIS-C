/*
 * A234040  a(n) = binomial(2*(n+1),n) * gcd(n,2)/(2*(n+1)).
 * https://oeis.org/A234040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   gcd, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(2*(n+1), n) * gcd(n, 2) / (2*(n+1))
 * Recurrence (derived from the formula):
 *   a(0) = 1
 *   For n ≥ 0:
 *     if n is even: a(n+1) = a(n) * (2n+3) / (n+3)
 *     if n is odd : a(n+1) = a(n) * 4*(2n+3) / (n+3)
 *
 * The implementation memoises computed terms and uses gcd
 * cancellations to keep intermediate products within the
 * limits of the underlying integer type (seq_t) for all n
 * whose results fit in seq_t.  The routine works for both
 * fixed-width and arbitrary-precision sec_t types.
 */
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mul_small.h"

static seq_t *cache = NULL;       /* memoised values */
static idx_t cache_size = 0;      /* current cache size */

/* Compute a(n) using memoisation and the recurrence */
seq_t a(idx_t n)
{
    /* initialise cache if this is the first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;     /* a(0) */
        cache_size = 1;
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to value n */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        idx_t idx = i - 1;          /* previous index (n) */

        seq_t num, den;

        if ((idx & 1ULL) == 0) {    /* even index : a(idx+1) = a(idx)*(2idx+3)/(idx+3) */
            num = (seq_t)(2 * idx + 3);
            den = (seq_t)(idx + 3);
        } else {                    /* odd index : a(idx+1) = a(idx)*4*(2idx+3)/(idx+3) */
            num = (seq_t)(8 * idx + 12);  /* 4*(2idx+3) */
            den = (seq_t)(idx + 3);
        }

        /* cancel gcd(num, den) */
        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        /* cancel gcd(prev, den) */
        seq_t g2 = gcd(prev, den);
        seq_t a_div = prev / g2;
        den /= g2;                       /* remaining denominator to cancel */

        /* cancel gcd(num, den) one more time; den must now be 1 */
        seq_t g3 = gcd(num, den);
        num /= g3;
        den /= g3;

        /* den should be 1 here */
        seq_t result = mul_small(a_div, (unsigned int)num);
        cache[i] = result;
    }

    cache_size = n + 1;
    return cache[n];
}
