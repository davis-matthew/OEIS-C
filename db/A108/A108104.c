/*
 * A108104  Sequence A000930 with terms repeated.
 * https://oeis.org/A108104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-6), with a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 119, u64 n <= 235,
 *            u128 n <= 467, big any n
 * Validated: standard, pass, 72 terms through n = 958 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static storage for already computed values */
    static seq_t *dp = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;

    /* first call: allocate and set the first six terms */
    if (dp == NULL) {
        cap = 64;                             /* initial capacity */
        dp = mem_new(seq_t, cap);
        if (dp == NULL) return (seq_t)0;     /* allocation error */
        for (idx_t i = 0; i < 6; ++i) dp[i] = (seq_t)1;
        size = 6;                          /* number of computed terms */
    }

    if (n < size) return dp[n];             /* already known */

    /* grow storage if necessary */
    while (n >= cap) {
        idx_t newcap = cap * 2;
        seq_t *tmp = mem_resize(seq_t, dp, newcap);
        if (tmp == NULL) return (seq_t)0;   /* allocation error */
        dp = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 6) {
            dp[i] = (seq_t)1;               /* base values */
        } else {
            dp[i] = dp[i-2] + dp[i-6];      /* recurrence */
        }
    }
    size = n + 1;
    return dp[n];
}
