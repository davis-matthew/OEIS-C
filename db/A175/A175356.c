/*
 * A175356  Those positive integers n where, when written in binary, there are
 *          exactly k number of runs (of either 0's or 1's) each of exactly k
 *          length, for all k where 1<=k<=m, for some positive integer m.
 * https://oeis.org/A175356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 9725, u64 n <= 9725,
 *            u128 n <= 9725, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A175356:
   numbers n such that in binary representation there exist runs of
   length k occurring exactly k times for all k = 1..m (some m).
   The function generates all such numbers for increasing m until
   the requested index is reached and returns the nth element
   (1‑based). */
#include <stdlib.h>

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Recursive generation of all permutations of the multiset
   {1^1, 2^2, …, m^m}.  Each permutation gives a binary
   representation with alternating 1s and 0s, starting with 1.
   'counts' holds remaining multiplicities of each run length. */
static void dfs_gen(int depth, seq_t val, int m, int *counts,
                    size_t *idx, seq_t *seq)
{
    int N = m * (m + 1) / 2;
    if (depth == N) {
        seq[*idx] = val;
        (*idx)++;
        return;
    }
    int bit = (depth % 2 == 0) ? 1 : 0;   /* 1 for 1‑runs, 0 for 0‑runs */
    for (int len = 1; len <= m; ++len) {
        if (counts[len] == 0) continue;
        counts[len]--;
        seq_t new_val = val << len;
        if (bit == 1) {
            seq_t mask = ((seq_t)1 << len) - 1;
            new_val |= mask;
        }
        dfs_gen(depth + 1, new_val, m, counts, idx, seq);
        counts[len]++;
    }
}

seq_t a(idx_t n)
{
    static seq_t seq[20000];          /* buffer for generated terms */
    static size_t seq_len = 0;        /* number of terms stored */
    static int next_m = 1;            /* next m to generate */

    if (n == 0) return 0;             /* undefined but safe */

    while (seq_len < (size_t)n) {
        int m = next_m++;
        int N = m * (m + 1) / 2;
        int counts[32];
        for (int k = 1; k <= m; ++k) counts[k] = k;

        dfs_gen(0, 0, m, counts, &seq_len, seq);
        /* sort all generated terms; the earlier terms are already
           smaller because their binary length is shorter.           */
        qsort(seq, seq_len, sizeof(seq_t), cmp_seq);
    }
    return seq[(size_t)n - 1];
}
