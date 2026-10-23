/*
 * A156159  Squares of the form k^2+(k+17)^2 with integer k.
 * https://oeis.org/A156159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A156159
   Squares of the form k^2 + (k+17)^2 with integer k
   Recurrence: a(n) = 34*a(n-3) - a(n-6) - 2312   for n > 6
   Initial values: a(1)=169, a(2)=289, a(3)=625,
                    a(4)=2809, a(5)=7225, a(6)=18769
   The function caches results so that any call order is supported.
   All arithmetic is performed in the type seq_t; for 64‑bit builds
   no overflow occurs for all n whose value fits in uint64_t. */

#include <stdlib.h>   /* for malloc, realloc, free, exit */

static const seq_t INITIAL[7] = {
    0,
    169,  /* a(1) */
    289,  /* a(2) */
    625,  /* a(3) */
    2809, /* a(4) */
    7225, /* a(5) */
    18769 /* a(6) */
};

/* Static cache for computed terms (1‑based indexing). */
static seq_t *CACHE = NULL;
static idx_t CACHE_SIZE = 0;

/* Compute a(n) for n >= 1.  The cache is expanded lazily. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1 */

    /* Expand cache as needed */
    if (n > CACHE_SIZE) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(CACHE, (new_size + 1) * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        CACHE = tmp;
        /* Fill new entries */
        for (idx_t i = CACHE_SIZE + 1; i <= new_size; ++i) {
            if (i <= 6) {
                CACHE[i] = INITIAL[i];
            } else {
                /* a(i) = 34*a(i-3) - a(i-6) - 2312 */
                CACHE[i] = ((seq_t)34) * CACHE[i - 3] - CACHE[i - 6] - 2312;
            }
        }
        CACHE_SIZE = new_size;
    }
    return CACHE[n];
}
