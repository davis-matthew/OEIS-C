/*
 * A053338  a(n) contains n digits (either '6' or '9') and is divisible by
 *          2^n.
 * https://oeis.org/A053338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A053338):
   a(1) = 6;
   for n > 1:
     a(n) = a(n-1) + 10^(n-1) * (6 + 3 * ((a(n-1) >> (n-1)) & 1))
   The bit test above is equivalent to “if a(n-1)/2^(n-1) is even then 6 else 9”.
   This implementation memorises previous terms so subsequent calls are O(1).
   It works for any width of seq_t (8‑ to 128‑bit unsigned or an arbitrary‑precision type). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* stores a(0) … a(n) ; index 0 is unused */
    static idx_t cache_cap = 0;    /* size of allocated array                    */
    static idx_t cached   = 0;    /* highest n that has been computed           */
    static seq_t pow10    = 0;    /* 10^cached ; used to build the next term   */

    if (n == 0) return (seq_t)0;   /* offset 1; n=0 is undefined, return 0 */

    /* initialise on first call */
    if (cached == 0) {
        cache_cap = 2;                /* indices 0 and 1     */
        cache     = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[1]  = (seq_t)6;         /* a(1) = 6           */
        cached    = 1;
        pow10     = (seq_t)10;        /* next pow10 is 10^(1) */
    }

    /* grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;    /* ensure space for indices 0..n */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        /* determine the next digit: 6 if (a(i-1)/2^(i-1)) even, else 9 */
        seq_t bit   = (cache[i - 1] >> (i - 1)) & 1;
        seq_t digit = (seq_t)(6 + 3 * bit);

        /* a(i) = a(i-1) + 10^(i-1) * digit */
        cache[i] = cache[i - 1] + pow10 * digit;

        /* prepare pow10 for the next iteration: 10^i */
        pow10 *= (seq_t)10;
    }

    cached = n;       /* all terms up to n are now stored */
    return cache[n];
}
