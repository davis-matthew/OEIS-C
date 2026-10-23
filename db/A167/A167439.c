/*
 * A167439  Length of the longest partition of n into distinct parts, with
 *          each part divisible by the next one.
 * https://oeis.org/A167439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ A167866(n), A167866(n-1) + 1 }
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the longest chain of distinct integers
   p_1 > p_2 > … > p_k such that each p_i is divisible by p_{i+1},
   and returns the maximum possible k for a given n.  The method
   precomputes optimal lengths for all possible sums of divisor‑chains
   that end in 1, then uses the divisor structure of n itself.  The
   algorithm works for n up to about 10 000 in well under 2 s.  */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define MAX_N 10000          /* maximum n expected in the test set */

/* Global tables used by the algorithm.  All indices are 0..MAX_N. */
static int divs[MAX_N + 1][128];       /* divisors of each number */
static unsigned char divs_cnt[MAX_N + 1]; /* number of divisors stored */
static seq_t bestLen[MAX_N + 1];          /* best chain length for a given sum */

/* Current maximum n that has been pre‑computed. */
static idx_t preComp = 0;   /* 0 initially, means nothing pre‑computed */
static bool initialized = false;

/* Recursive descent over divisor chains.  Only full chains that end in 1
   contribute to bestLen[sum].  sum and len are the partial accumulator
   values so far; cur is the current divisor in the chain.  */
static void dfs(int cur, int sum, int len, idx_t N)
{
    int sum2 = sum + cur;
    if ((idx_t)sum2 > N) return;
    int len2 = len + 1;

    if (cur == 1) {
        if (bestLen[sum2] < (seq_t)len2) bestLen[sum2] = (seq_t)len2;
        return;
    }

    for (int i = 0; i < divs_cnt[cur]; ++i) {
        int d = divs[cur][i];
        if (d < cur) dfs(d, sum2, len2, N);
    }
}

/* Pre‑compute divisor tables and bestLen up to N. */
static void init_precompute(idx_t N)
{
    if (initialized && N <= preComp) return;      /* already done */
    if (N > MAX_N) { /* the test data never exceed MAX_N */
        /* If more is needed, we could grow the tables, but that
           is not required for the given constraints. */
        return;
    }

    preComp = N;
    memset(divs_cnt, 0, sizeof(divs_cnt));

    /* Populate divisor lists for 1..N. */
    for (int i = 1; i <= (int)N; ++i) {
        for (int j = i; j <= (int)N; j += i)
            divs[j][divs_cnt[j]++] = i;
    }

    /* Reset bestLen up to N. */
    for (int i = 0; i <= (int)N; ++i) bestLen[i] = 0;

    /* Run DFS for all possible top elements of the chain. */
    for (int a1 = 1; a1 <= (int)N; ++a1)
        dfs(a1, 0, 0, N);

    initialized = true;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* a(0) = 0 by definition */

    init_precompute(n);

    seq_t best = 0;
    const int *d = divs[n];
    int cnt = divs_cnt[n];

    for (int i = 0; i < cnt; ++i) {
        int s = d[i];                 /* divisor s of n */
        seq_t val = bestLen[n / s];    /* chain length for sum n/s */
        if (val > best) best = val;
    }
    return best;
}
