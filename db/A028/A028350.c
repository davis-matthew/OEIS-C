/*
 * A028350  Expansion of -1/x + 6*3F2( 5/6, 1, 7/6; 3/2, 2; 108*x).
 * https://oeis.org/A028350
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 6*(6*n-1)*(6*n+1)/((n+1)*(2*n+1)) * a(n-1) Initial value:
 *            a(-1) = -1 Recurrence holds for n ≥ 0
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Euclidean algorithm for seq_t (handles signed types) */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* External function required by the harness */
seq_t a(idx_t n)
{
    /* a(-1) is the first term */
    if (n < -1) return 0;            /* behaviour for indices < -1 is irrelevant */

    /* map n to array index: cache[0] = a(-1), cache[1] = a(0), ... */
    idx_t idx = n + 1;

    /* static cache */
    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t last_n = -2;        /* last n that has been computed; -1 means only a(-1) stored */
    static size_t cap = 0;           /* current capacity of cache */

    /* initialize cache on first invocation */
    if (!cache) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* malloc failed */
        cache[0] = (seq_t)-1;          /* a(-1) */
        last_n = -1;
    }

    /* grow cache if needed */
    if ((size_t)idx >= cap) {
        size_t new_cap = (size_t)idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cap = new_cap;
    }

    /* if already computed, return */
    if (n <= last_n) {
        return cache[(size_t)idx];
    }

    /* compute missing terms from last_n+1 up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        /* previous value a(i-1) resides at cache[i] */
        seq_t prev = cache[(size_t)i];

        /* denominator (i+1)*(2*i+1) */
        seq_t denom = (seq_t)(i + 1) * ((seq_t)(2 * i + 1));

        /* reduce prev by gcd with denom */
        seq_t g = gcd_seq_t(prev, denom);
        seq_t cur = prev / g;
        denom /= g;

        /* multiplicative factors for the recurrence */
        const seq_t f0 = 6;
        const seq_t f1 = (seq_t)6 * i - 1;
        const seq_t f2 = (seq_t)6 * i + 1;

        /* apply factor f0 */
        g = gcd_seq_t(cur, denom);
        cur /= g;
        denom /= g;
        cur *= f0;

        /* apply factor f1 */
        g = gcd_seq_t(cur, denom);
        cur /= g;
        denom /= g;
        cur *= f1;

        /* apply factor f2 */
        g = gcd_seq_t(cur, denom);
        cur /= g;
        denom /= g;
        cur *= f2;

        /* final division to finish the recurrence */
        if (denom != 1) {
            cur /= denom;
        }

        /* store a(i) at cache[i+1] */
        cache[(size_t)(i + 1)] = cur;
    }

    last_n = n;
    return cache[(size_t)idx];
}
