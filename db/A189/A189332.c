/*
 * A189332  Number of nondecreasing arrangements of 9 numbers in 0..n with the
 *          last equal to n and each after the second equal to the sum of one
 *          or two of the preceding four.
 * https://oeis.org/A189332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189332: Number of nondecreasing arrangements of 9 numbers in 0..n
   with the last equal to n and each after the second equal to the sum of
   one or two of the preceding four (distinct indices). */
#include <stddef.h>

static seq_t dfs(idx_t pos, idx_t arr[9], idx_t n)
{
    if (pos == 9) {
        return (arr[8] == n) ? 1 : 0;
    }

    /* Number of preceding values available (up to 4) */
    idx_t k = pos < 4 ? pos : 4;
    idx_t pre_start = pos - k;

    /* Collect distinct candidate values */
    idx_t cand[10]; /* maximum 4 single + 6 double =10 */
    int candcnt = 0;

    /* sums of one previous element */
    for (idx_t i = 0; i < k; ++i) {
        idx_t v = arr[pre_start + i];
        int dup = 0;
        for (int t = 0; t < candcnt; ++t)
            if (cand[t] == v) { dup = 1; break; }
        if (!dup) cand[candcnt++] = v;
    }

    /* sums of two distinct previous elements */
    for (idx_t i = 0; i < k; ++i) {
        for (idx_t j = i + 1; j < k; ++j) {
            idx_t v = arr[pre_start + i] + arr[pre_start + j];
            if (v > n) continue; /* avoid exceeding the bound */
            int dup = 0;
            for (int t = 0; t < candcnt; ++t)
                if (cand[t] == v) { dup = 1; break; }
            if (!dup) cand[candcnt++] = v;
        }
    }

    seq_t total = 0;
    for (int t = 0; t < candcnt; ++t) {
        idx_t v = cand[t];
        /* enforce nondecreasing order */
        if (v < arr[pos - 1]) continue;
        if (v > n) continue;
        arr[pos] = v;
        total += dfs(pos + 1, arr, n);
    }
    return total;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 as per the sequence offset */
    if (n == (idx_t)-1) return 0;

    seq_t result = 0;
    idx_t arr[9];

    for (idx_t a1 = 0; a1 <= n; ++a1) {
        arr[0] = a1;
        for (idx_t a2 = a1; a2 <= n; ++a2) {
            arr[1] = a2;
            result += dfs(2, arr, n);
        }
    }
    return result;
}
