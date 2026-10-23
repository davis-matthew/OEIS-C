/*
 * A041273  Denominators of continued fraction convergents to sqrt(149).
 * https://oeis.org/A041273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041273: Denominators of convergents to sqrt(149)
   Recurrence: a(n) = 227164*a(n-9) + a(n-18)
   Initial values supplied for 0 ≤ n ≤ 17 as listed in the OEIS entry. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t max_idx = 0;      /* last index that is currently stored */
    static size_t alloc   = 0;      /* allocated capacity */

    /* Initialisation: store the first 18 terms (n = 0 … 17) */
    if (alloc == 0) {
        alloc   = 18;
        cache   = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */

        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)5;
        cache[3]  = (seq_t)29;
        cache[4]  = (seq_t)92;
        cache[5]  = (seq_t)305;
        cache[6]  = (seq_t)1617;
        cache[7]  = (seq_t)1922;
        cache[8]  = (seq_t)9305;
        cache[9]  = (seq_t)225242;
        cache[10] = (seq_t)910273;
        cache[11] = (seq_t)1135515;
        cache[12] = (seq_t)6587848;
        cache[13] = (seq_t)20899059;
        cache[14] = (seq_t)69285025;
        cache[15] = (seq_t)367324184;
        cache[16] = (seq_t)436609209;
        cache[17] = (seq_t)2113761020;

        max_idx = 17;
    }

    /* If the requested index is already known, return it */
    if ((idx_t)max_idx >= n) {
        return cache[n];
    }

    /* Ensure the array is large enough for the requested index */
    size_t required = (size_t)n + 1;
    if (alloc < required) {
        size_t new_alloc = alloc;
        while (new_alloc < required) new_alloc *= 2;  /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return 0;                           /* allocation failure */
        cache = tmp;
        alloc = new_alloc;
    }

    /* Compute the missing terms using the recurrence */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        seq_t term = ((seq_t)227164) * cache[i - 9] + cache[i - 18];
        cache[i] = term;
    }

    max_idx = n;
    return cache[n];
}
