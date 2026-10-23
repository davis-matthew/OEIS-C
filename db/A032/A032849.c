/*
 * A032849  Numbers whose base-2 representation Sum_{i=0..m} d(i)*2^i has d(m)
 *          <= d(m-1) >= d(m-2) <= ...
 * https://oeis.org/A032849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose binary representation has bits
 *            d(m) <= d(m-1) >= d(m-2) <= …, i.e. alternating ≤ and ≥
 *            comparisons starting from the most significant bit
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Memoisation table for counting remaining completions.
   Maximum bit‑length needed for n up to 64‑bit indices is <200. */
#define MAX_REM 200
static unsigned __int128 dp[MAX_REM+1][2][2];
static bool vis[MAX_REM+1][2][2];

/* Count the number of valid suffixes of length `rem` given
   previous bit `prev` and the comparison direction `le` (1 for ≤, 0 for ≥). */
static unsigned __int128 countWays(unsigned int rem, unsigned prev, unsigned le)
{
    if (rem == 0) return 1;
    if (vis[rem][prev][le]) return dp[rem][prev][le];
    vis[rem][prev][le] = true;
    unsigned __int128 res = 0;
    if (le) {                     /* need prev ≤ next */
        if (prev == 0) {
            res += countWays(rem-1, 0, !le);
            res += countWays(rem-1, 1, !le);
        } else {                  /* prev == 1 */
            res += countWays(rem-1, 1, !le);
        }
    } else {                      /* need prev ≥ next */
        if (prev == 1) {
            res += countWays(rem-1, 0, !le);
            res += countWays(rem-1, 1, !le);
        } else {                  /* prev == 0 */
            res += countWays(rem-1, 0, !le);
        }
    }
    dp[rem][prev][le] = res;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑offset usage */

    /* Find the bit‑length L that contains the n‑th term.
       Count(L) = Fibonacci number F_L (F_1=F_2=1).  */
    unsigned __int128 sum = 0;
    unsigned __int128 f_prev2 = 1, f_prev1 = 1;   /* F_1, F_2 */
    int L = 0;
    unsigned __int128 fib_val = 0;

    while (1) {
        ++L;
        if (L == 1) fib_val = 1;
        else if (L == 2) fib_val = 1;
        else {
            fib_val = f_prev2 + f_prev1;
            f_prev2 = f_prev1;
            f_prev1 = fib_val;
        }
        sum += fib_val;
        if (sum >= n) break;
    }

    /* Index (0‑based) within the block of length L numbers. */
    unsigned __int128 prev_sum = sum - fib_val;   /* terms before length L */
    unsigned __int128 k = n - prev_sum - 1;       /* 0‑based rank in this block */

    /* Reconstruct the number bit by bit. */
    seq_t result = 0;
    result |= ((seq_t)1 << (L-1));     /* most significant bit */

    unsigned prev_bit = 1;
    unsigned le = 1;                   /* first comparison is ≤ */

    for (int pos = L-2; pos >= 0; --pos) {
        bool chosen = false;
        for (unsigned cand = 0; cand <= 1; ++cand) {
            bool allowed = (le && prev_bit <= cand) || (!le && prev_bit >= cand);
            if (!allowed) continue;

            unsigned __int128 cnt = countWays((unsigned)pos, cand, !le);
            if (k < cnt) {
                if (cand) result |= ((seq_t)1 << pos);
                prev_bit = cand;
                le = !le;
                chosen = true;
                break;
            } else {
                k -= cnt;
            }
        }
        /* The loop guarantees a choice; this is a safety guard. */
        if (!chosen) return result;
    }

    return result;
}
