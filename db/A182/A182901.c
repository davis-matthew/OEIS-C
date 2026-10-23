/*
 * A182901  Number of weighted lattice paths in B(n) having no valleys. The
 *          members of B(n) are paths of weight n that start at (0,0), end on
 *          but never go below the horizontal axis, and whose steps are of the
 *          following four kinds: an (1,0)-step with weight 1, an (1,0)-step
 *          with weight 2, a (1,1)-step with we...
 * https://oeis.org/A182901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies (n+4)*a(n) - (n+1)*a(n-1) - 3*(n+2)*a(n-2) -
 *            (n+1)*a(n-3) - (n-2)*a(n-4) - 3*(n-3)*a(n-5) - (n-2)*a(n-6) +
 *            (n-5)*a(n-7) = 0 with a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=8,
 *            a(5)=17, a(6)=36
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated length */
    static size_t next  = 0;      /* first index yet uncomputed */
    static int  init    = 0;      /* whether base terms were set */

    /* Ensure we have storage for indices 0..n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 8) new_cap = 8;               /* enough for base terms */
        if (new_cap < cap * 2) new_cap = cap * 2;   /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                         /* memory exhausted */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base values once */
    if (!init) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)17;
        cache[6] = (seq_t)36;
        next    = 7;               /* next index to compute */
        init    = 1;
    }

    /* If n < next, we already have the answer */
    if (n < next) return cache[(size_t)n];

    /* Compute all needed terms up to n */
    for (size_t m = next; m <= (size_t)n; ++m) {
        /* All arithmetic uses seq_t operators (or built‑in integer type)
           so no truncation occurs. */
        seq_t rhs = 0;

        rhs += cache[m-1] * (seq_t)(m+1);
        rhs += cache[m-2] * (seq_t)(3*(m+2));
        rhs += cache[m-3] * (seq_t)(m+1);
        rhs += cache[m-4] * (seq_t)(m-2);
        rhs += cache[m-5] * (seq_t)(3*(m-3));
        rhs += cache[m-6] * (seq_t)(m-2);
        rhs -= cache[m-7] * (seq_t)(m-5);

        cache[m] = rhs / (seq_t)(m+4);   /* exact division */
    }

    next = (size_t)n + 1;
    return cache[(size_t)n];
}
