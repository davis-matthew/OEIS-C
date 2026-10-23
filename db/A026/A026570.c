/*
 * A026570  a(n) = A026568(n,n-1), also a(n) = number of integer strings
 *          s(0),...,s(n) counted by A026568 such that s(n)=1.
 * https://oeis.org/A026570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the 3-term linear recurrence (n+1)*a(n) -
 *            2*n*a(n-1) + (-3*n-1)*a(n-2) + 2*(2*n-3)*a(n-3) = 0 with
 *            a(1)=a(2)=1. This implements OEIS A026570. The implementation
 *            uses a static cache of size 10001 (safe for n up to about 10000)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for abort() */

seq_t a(idx_t n)
{
    /* A static cache big enough for all requested n (up to 10000). */
    static seq_t cache[10001];
    static idx_t last = 0;  /* highest index already computed */

    if (n == 0) {
        /* The sequence is defined starting at n=1; return 0 for n=0. */
        return 0;
    }

    /* If the cache is not yet initialized, set the base cases. */
    if (last == 0) {
        cache[1] = 1;
        if (sizeof(cache)/sizeof(cache[0]) > 2)
            cache[2] = 1;
        last = 2;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sn    = (seq_t)i;            /* i */
        seq_t sn1   = sn + 1;              /* i+1 */
        seq_t twon  = 2 * sn;              /* 2*i */
        seq_t threenp1      = 3 * sn + 1;  /* 3*i+1 */
        seq_t two2nminus3   = 2 * (2 * sn - 3); /* 2*(2*i-3) */

        seq_t rhs = twon * cache[i - 1]
                  + threenp1 * cache[i - 2]
                  - two2nminus3 * cache[i - 3];

        cache[i] = rhs / sn1; /* division is exact by the recurrence */
    }
    last = n; /* update last computed index */

    return cache[n];
}
