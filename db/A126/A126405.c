/*
 * A126405  Number of base 18 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126405: Number of base-18 n‑digit numbers with adjacent digits
   differing by two or less.  Recurrence:
     a(0) = 1
     a(1) = 18
     For n ≥ 2:
         Let dp[d] be the number of length‑(k‑1) sequences ending with digit d.
         Then dp_new[d] = dp[d] + dp[d‑1] + dp[d‑2] + dp[d+1] + dp[d+2]
            (terms omitted when indexes are out of range 0…17).
         a(k) = Σ_{d=0}^{17} dp_new[d].
   Memoised to allow arbitrary call order.                                               */
/* Note: the function uses only integer arithmetic operations on seq_t; these
   are defined by the build system.                                                     */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;              /* array of known a(n) values          */
static idx_t  memo_cap = 0;             /* current capacity of memo array      */
static idx_t  memo_max = 0;             /* largest n for which a(n) is stored */

/* Helper to ensure memo has capacity for indices up to n (inclusive)  */
static void ensure_memo_capacity(idx_t n)
{
    if (n + 1 > memo_cap)
    {
        idx_t new_cap = n + 1;
        seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_mem) abort();          /* out of memory: terminate */
        if (memo)
        {
            memcpy(new_mem, memo, memo_max * sizeof(seq_t));
            free(memo);
        }
        memo = new_mem;
        memo_cap = new_cap;
    }
}

/* Main function to compute a(n)                                             */
seq_t a(idx_t n)
{
    /* initialise memo for n==0  */
    if (!memo)
    {
        ensure_memo_capacity(0);
        memo[0] = (seq_t)1;
        memo_max = 0;
    }
    if (n == 0)
        return memo[0];

    if (n > memo_max)
    {
        /* compute values from memo_max+1 up to n   */
        seq_t dp[18], ndp[18];
        idx_t i, d, step;

        /* start with length 1   */
        for (d = 0; d < 18; ++d)
            dp[d] = (seq_t)1;

        if (memo_max < 1)
        {
            ensure_memo_capacity(1);
            seq_t sum0 = (seq_t)18;        /* a(1) */
            memo[1] = sum0;
            memo_max = 1;
        }
        else if (memo_max == 1)
        {
            /* already have a(1), ensure capacity for further steps */
            // nothing to do
        }

        for (step = 2; step <= n; ++step)
        {
            /* compute ndp[d] for current length = step   */
            for (d = 0; d < 18; ++d)
            {
                seq_t val = dp[d];         /* same digit */
                if (d >= 1) val += dp[d-1];
                if (d >= 2) val += dp[d-2];
                if (d <= 16) val += dp[d+1];
                if (d <= 15) val += dp[d+2];
                ndp[d] = val;
            }
            /* sum ndp to get a(step)  */
            seq_t sum = (seq_t)0;
            for (d = 0; d < 18; ++d)
                sum += ndp[d];

            /* store result  */
            ensure_memo_capacity(step);
            memo[step] = sum;
            if (step > memo_max) memo_max = step;

            /* copy ndp into dp for next iteration   */
            for (d = 0; d < 18; ++d)
                dp[d] = ndp[d];
        }
    }
    return memo[n];
}
