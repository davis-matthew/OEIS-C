/*
 * A059508  Transform of A059502 applied to sequence 5,6,7,...
 * https://oeis.org/A059508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4), with a(1)=5,
 *            a(2)=11, a(3)=29, a(4)=79. This is the recurrence from the OEIS
 *            entry
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: a[0] holds a(1), a[1] holds a(2), etc. */
    static seq_t *cache = NULL;
    static idx_t capacity = 0, max_n = 0;

    if (n < 1) return 0;               /* Undefined for n<1, return 0 */

    /* Initialise cache with first four terms if needed. */
    if (!cache) {
        /* Ensure capacity at least 4 or n, whichever is larger. */
        capacity = (n > 4) ? n : 4;
        cache = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        cache[0] = (seq_t)5;   /* a(1) */
        cache[1] = (seq_t)11;  /* a(2) */
        cache[2] = (seq_t)29;  /* a(3) */
        cache[3] = (seq_t)79;  /* a(4) */
        max_n = 4;             /* Highest index computed so far */
    }

    /* If the requested index is already computed, return it. */
    if (n <= max_n) {
        return cache[n-1];
    }

    /* Grow cache if necessary. */
    if ((idx_t)capacity < n) {
        idx_t new_cap = capacity;
        while (new_cap < n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc((void *)cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(1);  /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        idx_t idx = i - 1;           /* array index for a(i) */
        seq_t t1 = 6 * cache[idx-1]; /* 6*a(i-1) */
        seq_t t2 = 11 * cache[idx-2];/* 11*a(i-2) */
        seq_t t3 = 6 * cache[idx-3]; /* 6*a(i-3) */
        seq_t t4 = cache[idx-4];     /* a(i-4) */

        /* Perform the recurrence: 6*a(i-1) - 11*a(i-2) + 6*a(i-3) - a(i-4) */
        seq_t term = t1 - t2; /* potential underflow is safe because the result stays non‑negative */
        term += t3;
        term -= t4;

        cache[idx] = term;
    }

    max_n = n;
    return cache[n-1];
}
