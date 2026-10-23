/*
 * A145529  Numbers n such that there exists x in N : (x+1)^3-x^3=91*n^2.
 * https://oeis.org/A145529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A145529 is defined by the recurrence a(1) = 1 a(2) = 1453 a(n+2)
 *            = 1454*a(n+1) - a(n) for n >= 1 This recurrence is equivalent to
 *            a(n) = A145527(n)/8281. The function below computes terms using
 *            memoisation; it works for any seq_t type (unsigned __int128,
 *            arbitrary‑precision, etc.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Multiply a small integer by a seq_t value without overflow when seq_t
   is a small unsigned integer type.  For larger types, normal multiplication
   is used.  The branch that is not taken is not evaluated at run time. */
#define MUL_SM(a,m) ( \
    (sizeof(seq_t) <= sizeof(unsigned long long)) ? \
        (seq_t)((__int128)(a)*(m)) : (seq_t)((a)*(m)) )

seq_t a(idx_t n)
{
    if (n < 1)               /* behaviour for n < 1 is unspecified */
        return 0;

    static seq_t *cache = NULL;   /* cache[1] .. cache[cap] hold computed terms */
    static idx_t cap = 0;          /* largest index that has been computed */

    /* initialise cache on first call */
    if (!cache) {
        /* allocate space for indices 0..2 (we ignore index 0) */
        cache = (seq_t *)malloc(sizeof(seq_t) * 3);
        if (!cache) return 0;      /* allocation failure – return 0 */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1453;
        cap = 2;
    }

    /* if the requested index is already cached, return it */
    if (n <= cap)
        return cache[(size_t)n];

    /* extend the cache to hold result up to n */
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        if (!tmp)
            return 0;  /* allocation failure – return 0 */
        cache = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cap + 1; i <= n; ++i) {
        seq_t term_prev1 = cache[(size_t)(i - 1)];
        seq_t term_prev2 = cache[(size_t)(i - 2)];
        seq_t prod = MUL_SM(term_prev1, 1454);
        cache[(size_t)i] = prod - term_prev2;
    }

    cap = n;   /* update cache size */
    return cache[(size_t)n];
}
