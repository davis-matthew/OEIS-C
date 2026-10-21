/*
 * A231390  Number of (n+1) X (2+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal, diagonal and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A231390 – Number of (n+1)×3 arrays with no strict majority neighbour
    Formula (empirical):  a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3)   for n ≥ 6
    Base values:  a(1)=7, a(2)=8, a(3)=15, a(4)=20, a(5)=31
    (The recurrence also reproduces a(6)=52). */

#include <stdint.h>
#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Return 0 for n==0 – seldom needed but makes the function total */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;            /* array of values a[0..max_computed] */
    static idx_t  max_computed = 0;        /* highest n for which cache[i] is valid */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate space for indices 0..5 (base values up to a(5)) */
        idx_t init_len = 6;                /* number of elements to allocate (0..5) */
        cache = (seq_t *)malloc(sizeof(seq_t) * init_len);
        if (cache == NULL) return (seq_t)0; /* out‑of‑memory – unlikely in tests */

        cache[0] = (seq_t)0;              /* dummy 0‑th element */
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)20;
        cache[5] = (seq_t)31;
        max_computed = 5;
    }

    /* If the requested n is beyond what we have cached, extend the cache
       and compute missing values using the recurrence. */
    if (n > max_computed) {
        /* Allocate new storage for indices 0..n */
        idx_t new_len = n + 1;  /* new number of elements */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) return (seq_t)0;  /* out‑of‑memory – unlikely */

        cache = tmp;

        /* Compute values from max_computed+1 up to n (inclusive) */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t two_prev = cache[i - 1] * ((seq_t)2);
            seq_t two_prevprev = cache[i - 3] * ((seq_t)2);
            cache[i] = two_prev + cache[i - 2] - two_prevprev;
        }

        max_computed = n;
    }

    return cache[n];
}
