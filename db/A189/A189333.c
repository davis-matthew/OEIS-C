/*
 * A189333  Number of nondecreasing arrangements of 10 numbers in 0..n with
 *          the last equal to n and each after the second equal to the sum of
 *          one or two of the preceding four.
 * https://oeis.org/A189333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nondecreasing arrangements of 10 numbers in
 *            0..n with last = n and each term after the second equal to the
 *            sum of one or two of the preceding four (no repeats allowed)
 * Range:     u8 n <= 5, u16 n <= 95, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation is done by exhaustive backtracking over the 10 positions.
   Because the sequence length is constant, the branching factor stays small
   and the method runs comfortably within the time limit for n ≤ 10 000. */
#include <stddef.h>   /* for size_t */

static void dfs(idx_t n, int pos, idx_t *a, seq_t *cnt)
{
    if (pos == 10) {                      /* finished 10 terms */
        if (a[9] == n) (*cnt)++;          /* only count when last equals n */
        return;
    }

    if (pos == 0) {                       /* first element: 0..n */
        for (idx_t v = 0; v <= n; ++v) {
            a[0] = v;
            dfs(n, 1, a, cnt);
        }
        return;
    }

    if (pos == 1) {                       /* second element: >= first */
        for (idx_t v = a[0]; v <= n; ++v) {
            a[1] = v;
            dfs(n, 2, a, cnt);
        }
        return;
    }

    /* pos >= 2: choose next value as sum of one or two of the previous
       up to four elements, staying nondecreasing and ≤ n. */
    const idx_t min_val = a[pos-1];        /* need a[pos] >= previous value */
    idx_t cand[20];
    size_t csz = 0;

    int from = pos > 4 ? pos-4 : 0;        /* available previous indices */

    /* one-element sums */
    for (int i = from; i < pos; ++i) {
        idx_t v = a[i];
        if (v < min_val || v > n) continue;
        /* deduplicate */
        int dup = 0;
        for (size_t t = 0; t < csz; ++t)
            if (cand[t] == v) { dup = 1; break; }
        if (!dup) cand[csz++] = v;
    }

    /* two-element sums (distinct indices) */
    for (int i = from; i < pos; ++i) {
        for (int j = i + 1; j < pos; ++j) {
            idx_t v = a[i] + a[j];
            if (v < min_val || v > n) continue;
            int dup = 0;
            for (size_t t = 0; t < csz; ++t)
                if (cand[t] == v) { dup = 1; break; }
            if (!dup) cand[csz++] = v;
        }
    }

    for (size_t t = 0; t < csz; ++t) {
        a[pos] = cand[t];
        dfs(n, pos + 1, a, cnt);
    }
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t a10[10];
    dfs(n, 0, a10, &result);
    return result;
}
