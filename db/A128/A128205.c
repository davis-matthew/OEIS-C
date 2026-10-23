/*
 * A128205  a(n) = 2^(n-1)*A047240(n).
 * https://oeis.org/A128205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 8*a(n-3) - 16*a(n-4);
   Initial values: a(0)=0, a(1)=1, a(2)=4, a(3)=24 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array holding computed terms   */
    static size_t alloc   = 0;    /* current allocated size         */
    static size_t computed = 0;   /* index of first uncomputed term */

    /* Make sure we have room for the first term and for indexes 0..n */
    size_t needed = (size_t)n + 1;
    if (needed < 4) needed = 4;           /* at least allow n=3              */

    if (alloc < needed) {                /* expand storage if necessary   */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed              */
        cache = tmp;
        alloc = needed;
    }

    /* Initialise the base terms */
    if (computed == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)24;
        computed = 4;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 1] * 2 + cache[i - 3] * 8;
        val -= cache[i - 4] * 16;          /* safe: the result is non‑negative   */
        cache[i] = val;
    }

    /* Update the number of computed terms */
    if (computed < (size_t)n + 1)
        computed = (size_t)n + 1;

    return cache[(size_t)n];
}
