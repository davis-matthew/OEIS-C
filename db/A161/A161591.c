/*
 * A161591  The list of the B values in the common solutions to the 2
 *          equations 13*k + 1 = A^2, 17*k + 1 = B^2.
 * https://oeis.org/A161591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   B(1)=1, B(2)=16, recurrence B(t+2)=15*B(t+1)-B(t)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_CACHE 20000  /* must be a compile‑time constant */

seq_t a(idx_t n)
{
    /* index 0 is not defined in OEIS; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    static seq_t cache[MAX_CACHE];
    static idx_t cache_len = 0;

    /* initialise the first two terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;          /* B(1) */
        cache[1] = (seq_t)16;         /* B(2) */
        cache_len = 2;
    }

    if (n <= cache_len)
        return cache[n - 1];

    /* grow the cache up to n (assumes n <= MAX_CACHE) */
    for (idx_t i = cache_len; i < n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)15 - cache[i - 2];
    }
    cache_len = n;
    return cache[n - 1];
}
