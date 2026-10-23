/*
 * A041971  Denominators of continued fraction convergents to sqrt(508).
 * https://oeis.org/A041971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A041971 – Denominators of continued fraction convergents to sqrt(508).
 * Recurrence used: a(n) = coeff(n) * a(n-1) + a(n-2), with coeff(n) from the
 * continued fraction expansion of sqrt(508).
 * The coefficients repeat with period 32: a0=22, the rest as listed below.
 * This implementation caches computed terms to allow arbitrary call order.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t cf[33] = {
    (seq_t)22, (seq_t)1,  (seq_t)1,  (seq_t)5,  (seq_t)1,  (seq_t)14,
    (seq_t)5,  (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)4,  (seq_t)1,
    (seq_t)2,  (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)10, (seq_t)1,
    (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)1,  (seq_t)4,  (seq_t)3,
    (seq_t)1,  (seq_t)1,  (seq_t)5,  (seq_t)14, (seq_t)1,  (seq_t)5,
    (seq_t)1,  (seq_t)1,  (seq_t)44
};

static seq_t *cache = NULL;
static idx_t capacity = 0;
static idx_t last_computed = (idx_t)-1;

/* External linkage */
seq_t a(idx_t n)
{
    /* Handle trivial first terms directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Initialize cache if needed */
    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* Grow cache if necessary */
    if (n + 1 > capacity) {
        capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* reallocation failed */
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)          /* never reached because i >= 2 here */
            coeff = (seq_t)22;
        else
            coeff = cf[((i - 1) % 32) + 1];  /* coefficients repeat every 32 */

        cache[i] = coeff * cache[i - 1] + cache[i - 2];
    }

    last_computed = n;
    return cache[n];
}
