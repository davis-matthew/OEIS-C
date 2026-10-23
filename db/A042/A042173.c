/*
 * A042173  Denominators of continued fraction convergents to sqrt(611).
 * https://oeis.org/A042173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1. For n > 0 a(n) is the denominator of the n‑th
 *            convergent of √611, using the partial quotients: a0 = 24,
 *            a1..a14 = {1,2,1,1,4,2,1,2,4,1,1,2,1,48} (period 14). This
 *            recurrence yields the OEIS recurrence a(n) = 473852*a(n-14) –
 *            a(n-28) for n > 27
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: multiply two seq_t values safely using 128‑bit arithmetic if needed */
static inline seq_t mul_seq_t(seq_t x, seq_t y)
{
    return (seq_t)(((__int128)x) * y);
}

/* The sparsely stored period of partial quotients for √611 */
static const seq_t period[14] = {
    1, 2, 1, 1, 4, 2, 1, 2, 4, 1, 1, 2, 1, 48
};

seq_t a(idx_t n)
{
    /* Static cache of computed denominators */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of the cache array */
    static size_t size = 0;  /* number of terms currently stored */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;     /* out of memory, but unlikely */
        cache[0] = (seq_t)1;     /* a(0) */
        size = 1;
    }

    /* Extend cache if needed */
    if ((size_t)n + 1 > size) {
        /* The indices that we will compute are from size to n inclusive */
        for (size_t i = size; i <= (size_t)n; ++i) {
            /* Ensure capacity */
            if (i >= cap) {
                size_t newcap = cap * 2;
                seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newptr) return 0;  /* memory error */
                cache = newptr;
                cap = newcap;
            }

            seq_t term;
            if (i == 0) {
                term = (seq_t)1;       /* a(0) */
            } else {
                seq_t a_k = period[(i - 1) % 14];   /* partial quotient a_i */
                seq_t t1 = cache[i - 1];
                seq_t t2 = (i >= 2) ? cache[i - 2] : (seq_t)0;
                term = mul_seq_t(a_k, t1) + t2;     /* q_i = a_i*q_{i-1} + q_{i-2} */
            }

            cache[i] = term;
        }
        size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
