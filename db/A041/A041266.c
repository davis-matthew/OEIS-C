/*
 * A041266  Numerators of continued fraction convergents to sqrt(146).
 * https://oeis.org/A041266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 290*a(n-2) - a(n-4) with a(0)=12, a(1)=145, a(2)=3492,
 *            a(3)=42049
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)12;
    if (n == 1) return (seq_t)145;
    if (n == 2) return (seq_t)3492;
    if (n == 3) return (seq_t)42049;

    /* Ensure the cache is big enough. */
    size_t needed = (size_t)(n + 1);
    if (needed > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) abort();           /* allocation failure */
        cache = new_cache;
        cap   = needed;
    }

    /* Store the four initial values. */
    cache[0] = (seq_t)12;
    cache[1] = (seq_t)145;
    cache[2] = (seq_t)3492;
    cache[3] = (seq_t)42049;

    /* Compute subsequent terms using the recurrence. */
    const seq_t C290 = (seq_t)290;
    for (idx_t i = 4; i <= n; ++i) {
        cache[i] = C290 * cache[i-2] - cache[i-4];
    }

    return cache[n];
}
