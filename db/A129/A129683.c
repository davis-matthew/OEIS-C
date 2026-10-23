/*
 * A129683  Expansion of (1/(1-2x))*exp(2x/(1-2x)).
 * https://oeis.org/A129683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A129683
   Formula: a(n) = 2^n * n! * Σ_{k=0..n} (binom(n,k)/k!)
   Equivalent recurrence: a(0) = 1, a(1) = 4,
   a(n) = 4*n*a(n-1) - 4*(n-1)^2*a(n-2)   for n ≥ 2
*/

#include <stddef.h>
#include <stdlib.h>

/* Computes a(n) for arbitrary n (n ≥ 0).  Uses memoisation so that
   repeated calls in any order are efficient. */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;   /* array of values a(0)…a(count-1)   */
    static size_t cap     = 0;      /* capacity of the array              */
    static size_t count   = 0;      /* number of values actually stored   */

    /* enlarge the array if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap << 1 : 2;
        while (newcap <= (size_t)n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure – return a safe value */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise base cases (if they haven't been set yet) */
    if (count == 0) {
        cache[0] = 1;
        count = 1;
    }
    if (count == 1 && n >= 1) {
        cache[1] = 4;
        count = 2;
    }

    /* compute missing terms up to n */
    for (size_t i = count; i <= (size_t)n; ++i) {
        seq_t term1 = 4 * (seq_t)i * cache[i-1];
        seq_t term2 = 4 * (seq_t)(i-1) * (seq_t)(i-1) * cache[i-2];
        cache[i] = term1 - term2;
    }

    return cache[(size_t)n];
}
