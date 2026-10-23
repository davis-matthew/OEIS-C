/*
 * A124295  Number of free generators of degree n of symmetric polynomials in
 *          7-noncommuting variables.
 * https://oeis.org/A124295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124295: Number of free generators of degree n of symmetric
   polynomials in 7 noncommuting variables.
   The sequence satisfies a linear recurrence of order 6 derived from its
   rational generating function:
        a(1)=1, a(2)=1, a(3)=2, a(4)=6, a(5)=22, a(6)=92, a(7)=426
        for n>7:
           a(n)=21*a(n-1)-170*a(n-2)+669*a(n-3)-1314*a(n-4)+1157*a(n-5)-309*a(n-6)
   This recurrence is valid for all n≥8. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* holds values d[n] = a(n+1) for n≥0 */
static idx_t cache_size = 0;         /* number of values stored in cache */

seq_t a(idx_t n) {
    /* n≥1, offset 1 => we need d[n-1] where d[0]=a(1) */
    if (n == 0) return 0;          /* undefined for offset 1, but return 0 */

    size_t target_index = (size_t)(n - 1);  /* index into d array */

    /* Initialize cache with first 7 terms if not yet done */
    if (cache == NULL) {
        cache_size = 7;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1; /* a(1) */
        cache[1] = (seq_t)1; /* a(2) */
        cache[2] = (seq_t)2; /* a(3) */
        cache[3] = (seq_t)6; /* a(4) */
        cache[4] = (seq_t)22;/* a(5) */
        cache[5] = (seq_t)92;/* a(6) */
        cache[6] = (seq_t)426;/* a(7) */
    }

    /* Grow cache if needed */
    if (cache_size <= target_index) {
        size_t old_size = cache_size;
        size_t new_size = target_index + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
        /* Compute new terms using recurrence */
        size_t start = old_size;
        if (start < 7) start = 7;   /* recurrence uses indices ≥7 */
        for (size_t i = start; i < new_size; ++i) {
            /* positive contributions */
            seq_t pos = cache[i-1] * (seq_t)21
                      + cache[i-3] * (seq_t)669
                      + cache[i-5] * (seq_t)1157;
            /* negative contributions */
            seq_t neg = cache[i-2] * (seq_t)170
                      + cache[i-4] * (seq_t)1314
                      + cache[i-6] * (seq_t)309;
            cache[i] = pos - neg;   /* pos ≥ neg by the recurrence */
        }
    }

    return cache[target_index];
}
