/*
 * A219004  Unmatched value maps: number of 3 X n binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..2 3 X n array.
 * https://oeis.org/A219004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2) + 49*a(n-3) + 45*a(n-4) - 17*a(n-5) -
 *            15*a(n-6), a(1) = 1, a(2) = 36, a(3) = 328, a(4) = 2265, a(5) =
 *            16648, a(6) = 127401 (OEIS A219004, offset 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static memoisation table */
    static seq_t *cache = NULL;        /* 1‑based array of terms */
    static size_t cache_size = 0;      /* allocated size (in seq_t slots) */
    static idx_t  max_computed = 0;    /* highest n for which cache[] is valid */

    /* Handle trivial case */
    if (n == 0) return (seq_t)0;

    /* Ensure capacity for indexes up to n */
    if (cache_size < (size_t)(n + 1))
    {
        size_t new_size = (size_t)(n + 1) + 7;   /* a safety margin */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base values once (lowest index that we need is 1) */
    if (max_computed < 6)
    {
        /* fill indices 1..6 */
        if (cache_size < 7) abort();
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)328;
        cache[4] = (seq_t)2265;
        cache[5] = (seq_t)16648;
        cache[6] = (seq_t)127401;
        max_computed = (idx_t)6;
    }

    /* Return directly if already computed */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms using the linear recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* Compute positive and negative parts separately to avoid
           signed arithmetic or intermediate underflow.                    */
        seq_t pos = cache[i-1] * (seq_t)7
                  + cache[i-3] * (seq_t)49
                  + cache[i-4] * (seq_t)45;
        seq_t neg = cache[i-2] * (seq_t)3
                  + cache[i-5] * (seq_t)17
                  + cache[i-6] * (seq_t)15;
        cache[i] = pos - neg;      /* guaranteed non‑negative for valid data */
    }

    max_computed = n;
    return cache[n];
}
