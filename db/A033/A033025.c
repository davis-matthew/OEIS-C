/*
 * A033025  Numbers whose base-12 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑12 expansion has no run of digits with length
 *            < 2
 * Range:     u8 n <= 12, u16 n <= 200, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates the n‑th such number (1‑based) by enumerating
   base‑12 strings of increasing length and picking the lexicographic
   n‑th string using a simple DP that tracks whether the current run
   has length 1 (state 0) or ≥2 (state 1). */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* DP table: dp[r][s] = number of ways to fill r remaining digits
   when the current run length is 1 (state 0) or ≥2 (state 1).   */
#define MAX_REMAIN 500          /* should be enough for n up to ~10⁴ */
static bool dp_inited = false;
static seq_t dp[MAX_REMAIN + 1][2];   /* state 0 = run length 1, state 1 = run length ≥2 */

/* ------------------------------------------------------------------ */
static void init_dp(void)
{
    /* base case: zero remaining digits */
    dp[0][0] = (seq_t)0;   /* end of number with run length 1 → invalid */
    dp[0][1] = (seq_t)1;   /* end of number with run length ≥2 → valid */

    for (int r = 1; r <= MAX_REMAIN; ++r)
    {
        /* state 0: must choose same digit, then we are in state 1 */
        dp[r][0] = dp[r-1][1];

        /* state 1: choose same digit (stay in 1) or different digit (go to 0) */
        seq_t same   = dp[r-1][1];                 /* same digit */
        seq_t diff   = dp[r-1][0] * 11u;           /* 11 other digits */
        dp[r][1] = same + diff;
    }

    dp_inited = true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!dp_inited) init_dp();

    /* ------------------------------------------------------------------ */
    /* 1) Find minimal length L such that the cumulative count of all
       valid numbers of length ≤ L is ≥ n.                                */
    seq_t cum = (seq_t)0;          /* cumulative count up to previous length */
    seq_t target = (seq_t)n;
    int L;                         /* length of the number to construct */

    for (L = 2; L <= MAX_REMAIN + 1; ++L)
    {
        int rem = L - 1;                 /* positions after the first digit */
        seq_t countL = dp[rem][0] * 11u; /* first digit 1‑11 */
        if (cum + countL >= target)
            break;
        cum += countL;
    }

    /* 2) Compute position k within all numbers of length L (1‑based)    */
    idx_t k = (idx_t)(target - cum);    /* k ∈ [1, countL] */

    /* 3) Choose first digit by iterating over 1–11                          */
    int rem = L - 1;                    /* remaining positions after first digit */
    seq_t blockSize = dp[rem][0];       /* number of suffixes for each first digit */

    int curDigit = 1;                   /* placeholder */
    {
        int d;
        for (d = 1; d <= 11; ++d)
        {
            if (k > (idx_t)blockSize)
            {
                k -= (idx_t)blockSize;
            }
            else
            {
                curDigit = d;
                break;
            }
        }
    }

    seq_t value = (seq_t)curDigit;      /* leading digit */
    int state = 0;                      /* after first digit run length 1 */

    /* 4) Build the remaining digits */
    for (int remaining = rem; remaining > 0; --remaining)
    {
        for (int cand = 0; cand < 12; ++cand)
        {
            /* Skip impossible candidates when run length is 1 */
            if (state == 0 && cand != curDigit)
                continue;

            int nextState = (cand == curDigit) ? 1 : 0;

            seq_t cnt = dp[remaining-1][nextState];

            /* cnt always fits in idx_t when it is smaller than k,
               because if it were larger we would have taken this branch. */
            if (cnt < (seq_t)k)
            {
                k -= (idx_t)cnt;
                continue;
            }

            /* This candidate is the correct next digit */
            value = value * (seq_t)12u + (seq_t)cand;
            curDigit = cand;
            state = nextState;
            break;
        }
    }

    return value;
}
