/*
 * A043594  Numbers whose base-4 representation has exactly 3 runs.
 * https://oeis.org/A043594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-4 representation has exactly 3 runs
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*  The maximum number of base‑4 digits for 64‑bit values is 32;
    we set a higher bound to be safe for wider types. */
#define MAX_DIGITS 64

/*  Global buffers for the digit DP  */
static int g_digits[MAX_DIGITS];
static int g_len;

/*  Memoisation tables for the DP */
static seq_t memo[MAX_DIGITS][4][5][2];
static bool vis[MAX_DIGITS][4][5][2];

/*  Recursive DP: returns the number of integers with exactly
    three runs among the remaining positions.  */
static seq_t dfs(int pos, int run, int prev, int started, int tight)
{
    if (pos == g_len)
        return (started && run == 3) ? (seq_t)1 : (seq_t)0;

    if (run > 3)
        return (seq_t)0;

    if (!tight && vis[pos][run][prev][started])
        return memo[pos][run][prev][started];

    int limit = tight ? g_digits[pos] : 3;
    seq_t total = 0;

    for (int d = 0; d <= limit; ++d) {
        int newStarted = started || (d != 0);
        int newPrev    = prev;
        int newRun     = run;

        if (newStarted) {
            newPrev = d;
            if (prev == 4)            /* first non‑zero digit      */
                newRun = 1;
            else if (d != prev)       /* a new run starts         */
                newRun = run + 1;
        }

        int newTight = tight && (d == limit);
        total += dfs(pos + 1, newRun, newPrev, newStarted, newTight);
    }

    if (!tight) {
        vis[pos][run][prev][started] = true;
        memo[pos][run][prev][started] = total;
    }
    return total;
}

/*  Count how many integers 1 … x have exactly three runs in base‑4. */
static seq_t count_leq(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;

    int idx = 0;
    while (x > (seq_t)0) {
        g_digits[idx++] = (int)(x % (seq_t)4);
        x /= (seq_t)4;
    }

    /* reverse to most significant digit first */
    for (int i = 0, j = idx - 1; i < j; ++i, --j) {
        int tmp = g_digits[i];
        g_digits[i] = g_digits[j];
        g_digits[j] = tmp;
    }
    g_len = idx;

    /* reset memoisation caches */
    memset(vis, 0, sizeof(vis));

    return dfs(0, 0, 4, 0, 1);
}

/*  Main function required by the framework.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* behaviour below offset is irrelevant */

    /*  Determine a digit length large enough to contain n terms.  */
    unsigned long long L = 3;
    for (;;) {
        unsigned __int128 comb = (unsigned __int128)L * (L - 1) * (L - 2) / 6;
        unsigned __int128 cnt  = 27 * comb;
        if (cnt >= n)
            break;
        ++L;
    }

    /*  Upper bound 4^L  */
    seq_t high = (seq_t)1;
    for (unsigned long long i = 0; i < L; ++i)
        high *= (seq_t)4;

    seq_t low = (seq_t)1;

    /*  Binary search the smallest number with at least n terms ≤ value.  */
    while (low < high) {
        seq_t mid  = low + (high - low) / 2;
        seq_t cnt  = count_leq(mid);
        if (cnt >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
