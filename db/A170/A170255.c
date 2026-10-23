/*
 * A170255  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A170255: Number of reduced words of length n in the Coxeter group
 *  on 6 generators with (S_i)^2 = (S_i S_j)^4 = I.
 *
 *  The generating function is
 *
 *      G(t) = (1 + 2t + 2t^2 + … + 2t^40 + t^41) /
 *            (1 - 4t - 4t^2 - … - 4t^40 + 10t^41)
 *
 *  From the denominator we obtain the linear recurrence
 *
 *      a(n) = 4 · ∑_{k=1}^{40} a(n‑k)  – 10 · a(n‑41)      (n ≥ 42)
 *
 *  For the first 41 terms the numerator determines the values:
 *
 *      a(0) = 1
 *      a(n) = 2 + 4 · ∑_{i=0}^{n‑1} a(i)                 (1 ≤ n ≤ 40)
 *      a(41) = 1 + 4 · ∑_{i=1}^{40} a(i)
 *
 *  The function memoises the computed terms so that repeated calls
 *  (in any order) require only a linear amount of work.
 */

#include <stdlib.h>    /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache of computed values and current size */
    static seq_t *cache  = NULL;
    static size_t cached = 0;   /* number of terms currently stored */

    /* -------------------------------------------------------------
     * If the requested index is not yet in the cache, grow the
     * cache and compute all intermediate terms using the recurrences
     * above.  The growth doubles the size each time, which keeps
     * the amortised complexity linear in the highest index seen.
     * ------------------------------------------------------------- */

    /* Make sure the cache can hold a[n] */
    if (n >= cached) {
        /* Determine new capacity – at least twice as big as needed. */
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cached * 2)
            new_cap = cached * 2;
        if (new_cap == 0)           /* first allocation when cached == 0 */
            new_cap = 1;

        /* Reallocate memory – cast the result because seq_t may be
           an arbitrary‑precision type in C++ mode. */
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) abort();          /* allocation failure */
        cache = ptr;

        /* Fill the missing entries, starting from the current size. */
        /* The loop variable is a size_t so that the indexing is
           well defined even for large n. */
        for (size_t i = cached; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;  /* a(0) */
            } else if (i <= 40) {     /* 1 ≤ i ≤ 40 */
                seq_t sum = (seq_t)0;
                for (size_t j = 0; j < i; ++j) sum += cache[j];
                cache[i] = (seq_t)2 + (seq_t)4 * sum;
            } else if (i == 41) {     /* a(41) */
                seq_t sum = (seq_t)0;
                for (size_t j = 1; j <= 40; ++j) sum += cache[j];
                cache[41] = (seq_t)1 + (seq_t)4 * sum;
            } else {                  /* n ≥ 42 */
                seq_t sum = (seq_t)0;
                for (size_t k = 1; k <= 40; ++k) sum += cache[i - k];
                cache[i] = (seq_t)4 * sum - (seq_t)10 * cache[i - 41];
            }
        }

        cached = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
