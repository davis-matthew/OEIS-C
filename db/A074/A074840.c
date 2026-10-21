/*
 * A074840  Numerators a(n) of fractions slowly converging to sqrt(2): let
 *          a(1) = 0, b(n) = n - a(n); if (a(n) + 1) / b(n) < sqrt(2), then
 *          a(n+1) = a(n) + 1, else a(n+1)= a(n).
 * https://oeis.org/A074840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = ⌊n · (2 − √2)⌋ (computed by the recurrence below)
 * Range:     u8 n <= 26, u16 n <= 1323, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The build system supplies the exact definitions of idx_t and seq_t. */
/* No redeclarations are made here. */

seq_t a(idx_t n)
{
    /* Index 0 is unused; a(1) is the first term. */
    static seq_t *cache    = NULL;   /* cache[n] holds a(n) */
    static idx_t  cached   = 0;      /* highest index for which cache[*] is valid */
    static size_t capacity = 0;      /* allocated size of cache */

    /* For indices below the offset return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Initial allocation or expansion of the cache. */
    if (!cache) {
        capacity = (size_t)n + 1;
        cache = (seq_t*)calloc(capacity, sizeof(seq_t));
        if (!cache)
            return (seq_t)0;       /* allocation failure → safe value */
        cache[0] = (seq_t)0;       /* unused slot */
        cache[1] = (seq_t)0;       /* a(1) = 0 */
        cached   = 1;
    } else if (capacity <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;       /* allocation failure → safe value */
        /* initialize newly allocated entries */
        for (size_t i = capacity; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache    = tmp;
        capacity = new_cap;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = cached; i < n; ++i) {
        seq_t ai   = cache[i];
        seq_t bi   = (seq_t)i - ai;          /* b(i) = i - a(i)       */
        seq_t ap1  = ai + (seq_t)1;           /* a(i)+1                */
        seq_t left  = ap1 * ap1;              /* (a(i)+1)^2            */
        seq_t right = (seq_t)2 * bi * bi;     /* 2·b(i)^2              */

        if (left < right)
            cache[i + 1] = ai + (seq_t)1;     /* a(i+1) = a(i)+1        */
        else
            cache[i + 1] = ai;                /* a(i+1) = a(i)          */
    }
    cached = n;

    return cache[n];
}
