/*
 * A130646  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+727)^2 = y^2.
 * https://oeis.org/A130646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1454 (n>6) a(1)=0, a(2)=56,
 *            a(3)=1925, a(4)=2181, a(5)=2465, a(6)=13056
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* array storing computed terms */
static size_t capacity = 0;       /* allocated size of cache */
static size_t computed = 0;       /* maximum n for which cache[n-1] is valid */

/* Ensure cache has space for at least n terms and initial 6 terms are set */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        /* first allocation: at least 6 terms, or n if larger */
        capacity = n > 6 ? n : 6;
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!cache) return;                     /* memory failure, caller handles 0 */
        cache[0] = 0;
        cache[1] = 56;
        cache[2] = 1925;
        cache[3] = 2181;
        cache[4] = 2465;
        cache[5] = 13056;
        computed = 6;
    } else if (capacity < (size_t)n) {
        /* extend allocation to size n */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return;                     /* memory failure, caller handles 0 */
        /* initialize newly allocated elements to 0 */
        for (size_t i = capacity; i < (size_t)n; ++i) tmp[i] = 0;
        cache = tmp;
        capacity = n;
    }
}

/* Compute a(n) using the recurrence, memoising results */
seq_t a(idx_t n) {
    if (n < 1) return 0;                    /* undefined for n=0, fallback */

    ensure_cache(n);
    if (computed >= (size_t)n) return cache[n-1];

    /* compute from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 6*a(i-3) - a(i-6) + 1454 */
        seq_t val = 6 * cache[(size_t)(i-4)]      /* a(i-3) */
                 - cache[(size_t)(i-7)]           /* a(i-6) */
                 + 1454;
        cache[(size_t)(i-1)] = val;
    }

    computed = n;
    return cache[n-1];
}
