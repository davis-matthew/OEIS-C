/*
 * A061828  Number of partitions of n into parts using the digits of n.
 * https://oeis.org/A061828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts using the digits of n.
   For each n, let D be the multiset of decimal digits of n.
   A part is allowed iff its decimal digits form a multiset that
   does not exceed the counts in D (no leading zero).  The value
   a(n) is the number of unordered partitions of n into such parts.
   The function caches results for each n for efficiency. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* compare two idx_t values for qsort */
static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* generate all numbers ≤ limit that can be made from the remaining
   digit counts in cnt[]. Each digit may be used at most its remaining
   count, no leading zero is allowed.  The numbers are stored in out[]. */
static void gen_allowed(idx_t curr, idx_t limit, int cnt[10], idx_t *out, size_t *cnt_out)
{
    if (curr != 0) {
        if (curr <= limit)
            out[(*cnt_out)++] = curr;
    }
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] == 0) continue;
        if (curr == 0 && d == 0) continue;            /* no leading zero */
        idx_t next = curr * 10 + d;
        if (next > limit) continue;
        cnt[d]--;
        gen_allowed(next, limit, cnt, out, cnt_out);
        cnt[d]++;                                      /* backtrack */
    }
}

/* cache of computed a(n) */
static seq_t *memo = NULL;        /* memo[n] holds a(n) */
static size_t memo_len = 0;       /* current size of memo */

/* main function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* not used, but defined */

    /* grow memo array if necessary */
    if (n >= memo_len) {
        size_t newlen = n + 1;
        seq_t *newp = mem_resize(seq_t, memo, newlen);
        if (!newp) return 0;              /* out of memory unlikely */
        for (size_t i = memo_len; i < newlen; ++i) newp[i] = 0;
        memo = newp;
        memo_len = newlen;
    }
    if (memo[n] != 0) return memo[n];     /* already known */

    /* compute digit counts of n */
    int cnt[10] = {0};
    idx_t tmp = n;
    while (tmp > 0) {
        ++cnt[tmp % 10];
        tmp /= 10;
    }

    /* generate all allowed parts ≤ n */
    static idx_t allowed[512];
    size_t allow_cnt = 0;
    gen_allowed(0, n, cnt, allowed, &allow_cnt);

    /* sort parts to ensure DP order */
    qsort(allowed, allow_cnt, sizeof(idx_t), cmp_idx);

    /* DP array for partitions */
    seq_t *dp = mem_new(seq_t, (size_t)n + 1);
    if (!dp) return 0;          /* out of memory unlikely */
    dp[0] = 1;
    for (size_t i = 0; i < allow_cnt; ++i) {
        idx_t part = allowed[i];
        for (idx_t s = part; s <= n; ++s) {
            dp[s] += dp[s - part];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    memo[n] = result;
    return result;
}
