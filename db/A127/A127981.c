/*
 * A127981  a(n) = (n + 1/3)*2^(n-1) - 1/2 + (-1)^(n-1)*(1/6).
 * https://oeis.org/A127981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6*n+2)*2^(n-1) + (-1)^(n-1) - 3)/6
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived recurrence: a(1)=1, for n>=2
   a(n)=2*a(n-1)+{ 2^(n-1)+1  if n is odd
                  2^(n-1)   if n is even } */
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset handling, not used */

    /* Static cache to allow repeated queries in any order. */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Grow cache if needed. */
    if (n + 1U > cap) {
        size_t new_cap = n + 1U;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */ abort(); }
        cache = tmp;
        /* Initialise new entries to zero. */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* Initialise first term. */
    if (cap > 1U && cache[1] == 0) cache[1] = 1;

    /* Compute required terms in order. */
    for (size_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;     /* already computed (maybe earlier call) */
        seq_t prev = cache[i-1];
        seq_t pow   = pow2(i-1);          /* 2^(i-1) */
        seq_t add   = pow;
        if ((i & 1U) == 1U) add = add + 1;/* odd i */
        seq_t next = (prev << 1) + add;   /* 2*prev + add */
        cache[i] = next;
    }

    return cache[n];
}
