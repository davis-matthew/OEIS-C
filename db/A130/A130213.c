/*
 * A130213  Order of modular group of degree 3^(n-1) + 1.
 * https://oeis.org/A130213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1)*(3^(2*n-2)-1)/2 Recurrence: a(1)=0, a(2)=12,
 *            a(n)=30*a(n-1)-81*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for sequence A130213.
   The function is memoised in a static array of size 10001,
   which is sufficient for the required test range (n up to ~10 000). */
seq_t a(idx_t n)
{
    /* Static cache: index 0 unused, cache[1] holds a(1). */
    static seq_t cache[10001];
    static idx_t max_computed = 0;   /* highest index whose value is known */

    /* Initialise the base cases once. */
    if (max_computed < 2) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        max_computed = 2;
    }

    /* Handle trivial requests. */
    if (n <= max_computed)
        return cache[n];

    /* Extend the cache up to the requested index using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)30 * cache[i - 1] - (seq_t)81 * cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
