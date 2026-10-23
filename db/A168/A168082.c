/*
 * A168082  Fibonacci 11-step numbers.
 * https://oeis.org/A168082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 28, u32 n <= 44, u64 n <= 76, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168082: Fibonacci 11‑step numbers
   Formula: a(n) = a(n‑1)+…+a(n‑11) for n>11
   Initial values: a(1) … a(10) = 0, a(11) = a(12) = 1
   Recurrence alternative: a(n) = 2*a(n‑1) – a(n‑12) for n>12 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;    /* allocated size (in terms) */
    static idx_t computed = 0;    /* maximum n already computed (0 if none) */

    /* Ensure there is space for at least n terms */
    if (n > capacity) {
        idx_t new_cap = (capacity) ? capacity : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure – fall back to 0 */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 10) {                /* a(1)…a(10) = 0 */
            cache[i-1] = 0;
        } else if (i == 11 || i == 12) {   /* a(11) = a(12) = 1 */
            cache[i-1] = 1;
        } else {                     /* a(i) = 2*a(i-1) – a(i-12) */
            cache[i-1] = 2 * cache[i-2] - cache[i-13];
        }
    }
    computed = n;

    return cache[n-1];
}
