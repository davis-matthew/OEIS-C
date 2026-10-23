/*
 * A161037  Number of partitions of n into Fibonacci numbers where every part
 *          appears at least 13 times.
 * https://oeis.org/A161037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into Fibonacci parts such that
 *            every part that appears occurs at least 13 times
 * Range:     u8 n <= 117, u16 n <= 315, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_one, seq_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/seq_zero.h"

static seq_t* alloc_arr(idx_t sz) {
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t*)malloc((size_t)sz * sizeof(seq_t));
#endif
}
static void free_arr(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Return 1 as a seq_t, guaranteed to be the multiplicative identity. */

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The first 12 integers cannot be expressed with parts of size 13 or
       more copies of any Fibonacci number. */
    if (n < 13) return seq_zero();

    /* Build the list of Fibonacci numbers not exceeding n.
       Fibonacci sequence used: 1,2,3,5,8,...  (unique, no duplicate 1). */
    uint64_t fibs[32];
    int fibcnt = 0;
    fibs[fibcnt++] = 1;
    fibs[fibcnt++] = 2;
    while (1) {
        uint64_t next = fibs[fibcnt-1] + fibs[fibcnt-2];
        if (next > (uint64_t)n) break;
        fibs[fibcnt++] = next;
    }

    idx_t N = n;
    idx_t sz = N + 1;

    seq_t *dp = alloc_arr(sz);
    if (!dp) return seq_zero();          /* allocation failed */

    /* Initialise dp: exactly one way to make 0, none for >0. */
    for (idx_t i = 0; i < sz; ++i) dp[i] = seq_zero();
    dp[0] = seq_one();

    /* Process each Fibonacci part.  For a part of size f, we may use it
       zero times or at least 13 times.  The following implements the
       standard unbounded knapsack DP with a sliding–window trick. */
    for (int fi = 0; fi < fibcnt; ++fi) {
        uint64_t f = fibs[fi];
        seq_t *newdp = alloc_arr(sz);
        if (!newdp) { free_arr(dp); return seq_zero(); }

        /* Start with the contribution of omitting this part. */
        for (idx_t i = 0; i < sz; ++i) newdp[i] = dp[i];

        /* For each residue class modulo f, add contributions of
           at least 13 copies of the part. */
        for (idx_t r = 0; r < f && r <= N; ++r) {
            seq_t pref = seq_zero();      /* running sum of dp values
                                            that can be extended. */
            idx_t t = 0;                  /* count of f's used so far */
            idx_t s = r;                  /* current sum (r + t*f) */

            while (s <= N) {
                if (t >= 13) {
                    /* dp[s - 13*f] corresponds to using one less
                       block of size f (i.e. t-13 copies). */
                    pref += dp[r + (t - 13) * f];
                }
                newdp[s] += pref;
                ++t;
                s += f;
            }
        }

        free_arr(dp);
        dp = newdp;
    }

    seq_t result = dp[N];
    free_arr(dp);
    return result;
}
