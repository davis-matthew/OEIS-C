/*
 * A125181  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n whose ascent lengths form the k-th partition of the
 *          integer n; the partitions of n are ordered in the "Mathematica"
 *          ordering.
 * https://oeis.org/A125181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the n‑th term of the flattened triangle of T(n,k). For
 *            each row m the partitions of m are listed in Mathematica order.
 *            For a partition p = [a(1)^{e(1)}, …, a(j)^{e(j)}] with k =
 *            e(1)+…+e(j) the corresponding triangle entry is (P) T(m,k) = m!
 *            / [ (m‑k+1)! · e(1)! e(2)! … e(j)! ]. The funct...
 * Range:     u8 n <= 27, u16 n <= 452, u32 n <= 5562, u64 n <= 9222,
 *            u128 n <= 9222, big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required definitions (provided by the build system) */
#include <stdbool.h>
#include "../utils/binom_n_k.h"

/* Local helper: factorial for small m (m ≤ n). */
static seq_t fact_small(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i) r *= i;
    return r;
}

/* Compute T(m,k) for a partition given by parts[0..k-1] (non‑increasing). */
static seq_t compute_T(idx_t m, idx_t k, int *parts)
{
    seq_t res = binom_n_k(m, k - 1);          /* C(m, k-1) */
    /* multiply by (k-1)! */
    for (idx_t i = 2; i <= k - 1; ++i) res *= i;

    /* divide by factorials of multiplicities */
    idx_t curr = parts[0];
    idx_t cnt  = 1;
    for (idx_t i = 1; i < (idx_t)k; ++i) {
        if (parts[i] == curr) {
            ++cnt;
        } else {
            for (idx_t j = 2; j <= cnt; ++j) res /= j;
            curr = parts[i];
            cnt  = 1;
        }
    }
    for (idx_t j = 2; j <= cnt; ++j) res /= j;
    return res;
}

/* Recursive enumeration of partitions of `rem` with largest part ≤ `max`,
   filling parts[depth..] with the current partial partition. */
static void gen_partitions(idx_t m, idx_t rem, idx_t max,
                           int *parts, idx_t depth,
                           idx_t *cur_idx, idx_t target,
                           bool *found, seq_t *result)
{
    if (*found) return;
    if (rem == 0) {                       /* a complete partition */
        if (*cur_idx == target) {
            *result = compute_T(m, depth, parts);
            *found  = true;
        }
        (*cur_idx)++;
        return;
    }
    for (idx_t part = (rem < max ? rem : max); part >= 1; --part) {
        if (*found) return;
        parts[depth] = (int)part;
        gen_partitions(m, rem - part, part,
                       parts, depth + 1,
                       cur_idx, target, found, result);
    }
}

/* Public function requested by the benchmark. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n==0, but not used */

    idx_t cur_idx = 1;              /* flattened index, starting at 1 */
    bool  found   = false;
    seq_t result  = 0;

    int parts[128];                 /* max parts for rows of interest (≤10000) */

    for (idx_t row = 1; !found; ++row) {
        gen_partitions(row, row, row,
                       parts, 0,
                       &cur_idx, n, &found, &result);
    }
    return result;
}
