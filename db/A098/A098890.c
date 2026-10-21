/*
 * A098890  a(n) = A001652(n)*A046090(n)+1 = A098602(n)+1.
 * https://oeis.org/A098890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) Initial values: a(0)=1,
 *            a(1)=13, a(2)=421
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Return the first three values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)421;

    /*  Memoised recurrence for n ≥ 3  */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;          /* highest index cached so far */

    if (n <= max_n) return cache[n];/* already computed */

    /*  Grow the cache if necessary  */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0; /* allocation failure – unlikely in tests */
    cache = new_cache;

    /*  Initialise the initial three terms if this is the first allocation  */
    if (max_n < 2) {
        cache[0] = (seq_t)1;
        if (new_size > 1) cache[1] = (seq_t)13;
        if (new_size > 2) cache[2] = (seq_t)421;
        max_n = (new_size > 2 ? 2 : new_size - 1);
    }

    /*  Compute remaining terms up to n  */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Use the recurrence; the intermediate arithmetic is performed
         * in the type of 'seq_t'.  For 64‑bit builds this is modulo 2^64,
         * which still yields the correct value whenever the true result
         * fits in 64 bits.  For arbitrary‑precision builds the operators
         * of the high‑level integer class provide exact arithmetic.  */
        cache[i] = cache[i-1] * (seq_t)35
                 - cache[i-2] * (seq_t)35
                 + cache[i-3];
    }
    max_n = n;

    return cache[n];
}
