/*
 * A143457  Expansion of 1/(x^k*(1-x-3*x^(k+1))) for k=6.
 * https://oeis.org/A143457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-7), a(0)=1, a(1)=4, a(2)=7, a(3)=10,
 *            a(4)=13, a(5)=16, a(6)=19
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 62, u64 n <= 128,
 *            u128 n <= 260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build. */
static seq_t *a_cache   = NULL;    /* memoisation array */
static size_t a_cap     = 0;       /* allocated capacity of a_cache */
static idx_t  a_computed = 6;      /* largest index that has been computed */

/* Ensure a_cache can hold elements up to index n (inclusive). */
static void ensure_capacity(idx_t n) {
    size_t needed = (size_t)(n + 1);
    if (a_cap >= needed) return;

    size_t new_cap = a_cap ? a_cap : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_cache = tmp;

    /* initialise newly allocated elements to zero */
    for (size_t i = a_cap; i < new_cap; ++i) a_cache[i] = 0;
    a_cap = new_cap;
}

/* Compute a(n), the n-th term of OEIS A143457. */
seq_t a(idx_t n) {
    /* First‑time initialisation. */
    if (!a_cache) {
        a_cap = 8;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < a_cap; ++i) a_cache[i] = 0;
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)4;
        a_cache[2] = (seq_t)7;
        a_cache[3] = (seq_t)10;
        a_cache[4] = (seq_t)13;
        a_cache[5] = (seq_t)16;
        a_cache[6] = (seq_t)19;
        a_computed = 6;
    }

    if (n > a_computed) {
        ensure_capacity(n);
        /* Compute successive terms up to n. */
        for (idx_t i = a_computed + 1; i <= n; ++i) {
            if (i >= 7)
                a_cache[i] = a_cache[i-1] + (seq_t)3 * a_cache[i-7];
            /* indices 0..6 already have the required initial values */
        }
        a_computed = n;
    }

    return a_cache[n];
}
