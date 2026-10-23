/*
 * A164427  Number of binary strings of length n with no substrings equal to
 *          0000 0011 or 0110.
 * https://oeis.org/A164427
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 84, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164427: Number of binary strings of length n with no substrings
   equal to 0000, 0011 or 0110.
   Recurrence (for n ≥ 9):
     a(n) = a(n-1) + a(n-2) + a(n-4) – a(n-5)
   Base values:
     a(4) = 13, a(5) = 22, a(6) = 37, a(7) = 63, a(8) = 105
   For n < 4 the function returns 0.  The implementation memoises the
   computed terms in a static dynamic array and grows it as needed.  It
   works for arbitrary‑precision seq_t as well as for fixed‑size unsigned
   integer types.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;

    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;     /* allocated array size (elements 0..cap-1) */
    static idx_t    computed = 0;  /* highest index that has a valid value */

    /* we need storage for indices up to at least 8 because of the base
       values, and up to n when requested.  */
    size_t needed = (size_t)(n >= 8 ? n + 1 : 9);
    if (cap < needed)
    {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        /* initialise newly allocated slots to zero */
        for (size_t i = cap; i < needed; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = needed;
    }

    /* initialise base values once */
    if (computed == 0)
    {
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)22;
        cache[6] = (seq_t)37;
        cache[7] = (seq_t)63;
        cache[8] = (seq_t)105;
        computed = 8;
    }

    if (n <= computed) return cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        seq_t t = cache[i - 1];
        t += cache[i - 2];
        t += cache[i - 4];
        t -= cache[i - 5];
        cache[i] = t;
    }
    computed = n;
    return cache[n];
}
