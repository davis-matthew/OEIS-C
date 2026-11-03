/*
 * A189321  Number of nondecreasing arrangements of n+2 numbers in 0..4 with
 *          the last equal to 4 and each after the second equal to the sum of
 *          one or two of the preceding four.
 * https://oeis.org/A189321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + 11*n - 32 for n > 5, else computed by brute force
 * Range:     u8 n <= 12, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 65 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* return true if value 'val' can appear at position 'pos' in the sequence,
   given the preceding elements in 'seq' (0‑based index). */
static bool is_allowed_value(const seq_t *seq, idx_t pos, seq_t val)
{
    idx_t start = (pos > 4) ? pos - 4 : 0;
    for (idx_t i = start; i < pos; ++i)
    {
        if (seq[i] == val) return true;
        for (idx_t j = i + 1; j < pos; ++j)
        {
            seq_t sum = seq[i] + seq[j];
            if (sum == val) return true;
        }
    }
    return false;
}

/* depth‑first search counting all admissible sequences of length L.
   'seq' holds the current sequence, 'count' accumulates the results. */
static void dfs(idx_t pos, idx_t L, seq_t *seq, seq_t *count)
{
    if (pos == L)
    {
        if (seq[pos - 1] == 4) (*count)++;
        return;
    }

    seq_t prev = (pos == 0) ? 0 : seq[pos - 1];
    idx_t start = (idx_t)prev;
    for (idx_t v = start; v <= 4; ++v)
    {
        seq_t val = (seq_t)v;
        if (pos >= 2 && !is_allowed_value(seq, pos, val))
            continue;
        seq[pos] = val;
        dfs(pos + 1, L, seq, count);
    }
}

seq_t a(idx_t n)
{
    if (n <= 5)
    {
        idx_t L = n + 2;
        seq_t seq[10];
        seq_t count = 0;
        dfs(0, L, seq, &count);
        return count;
    }

    seq_t N = (seq_t)n;
    seq_t res = N * N + (seq_t)11 * N - 32;
    return res;
}
