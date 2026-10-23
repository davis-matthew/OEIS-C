/*
 * A021760  Decimal expansion of 1/756.
 * https://oeis.org/A021760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   recurrence a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) (n
 *            > 6) a(0) = 0, a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 2, a(5) = 2,
 *            a(6) = 7
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
static seq_t *cache = nullptr;
#else
static seq_t *cache = NULL;
#endif
static size_t cap = 0;          /* allocated size (number of elements) */
static idx_t max_computed = 0;  /* highest index that has been filled */
static int initialized = 0;     /* 0=no base terms stored yet */

/* Grow the cache so that indices 0..n are valid.  The array is
   resized only upwards.  Elements are default‑constructed (by new[]
   in C++ mode or left uninitialised in C mode; they will be assigned
   before use). */
static void ensure_capacity(idx_t n)
{
    if (cap <= (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 7) new_cap = 7;          /* needs 0..6 at least */

        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_cap];
#else
        tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (!tmp) exit(EXIT_FAILURE);

        /* copy old contents */
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];

#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Make the buffer big enough for the requested index. */
    ensure_capacity(n);

    /* Initialise the first 7 terms only once. */
    if (!initialized) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)2;
        cache[6] = (seq_t)7;
        max_computed = 6;
        initialized  = 1;
    }

    /* If the term has already been computed, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Compute any missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = a(i-1) - a(i-2) + a(i-3) - a(i-4) + a(i-5) */
        seq_t pos = cache[i-1] + cache[i-3] + cache[i-5];
        seq_t neg = cache[i-2] + cache[i-4];
        cache[i] = pos - neg;   /* always non‑negative for this sequence */
    }
    max_computed = n;
    return cache[n];
}
