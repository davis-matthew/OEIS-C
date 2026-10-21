/*
 * A050052  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 4.
 * https://oeis.org/A050052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 193, u64 n <= 768,
 *            u128 n <= 3618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = 2, a(3) = 4.
   For n >= 4:
      p = floor(log2(n-2))
      m = 2*n - 3 - 2^(p+1)
      a(n) = a(n-1) + a(m)
   The sequence is computed iteratively and memoised so that any call
   order is supported.
*/
#include <stdlib.h>

static void ensure_cap(idx_t n, seq_t **cache, size_t *cap) {
    if (*cap < (size_t)(n + 1)) {
        size_t newcap = (size_t)(n + 1);
        *cache = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        *cap   = newcap;
    }
}

seq_t a(idx_t n)
{
    /* static dynamic cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* length of allocated array */
    static size_t  last  = 0;   /* last index computed (0 if none) */

    /* Handle trivial cases directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Ensure the cache is large enough */
    ensure_cap(n, &cache, &cap);

    /* Initialise base values if not done yet */
    if (last == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        last = 3;
    }

    /* Compute values up to n */
    for (idx_t i = (idx_t)last + 1; i <= n; ++i) {
        int p = 0;
        idx_t temp = i - 2;
        while (temp >>= 1) p++;             /* p = floor(log2(i-2)) */
        idx_t pow2 = ((idx_t)1) << (p + 1);  /* 2^(p+1) */
        idx_t m    = 2 * i - 3 - pow2;       /* 1 <= m <= i-2 */
        cache[i] = cache[i - 1] + cache[m];
    }
    last = n;
    return cache[n];
}
