/*
 * A051707  Number of factorizations of (n,n) into pairs (j,k).
 * https://oeis.org/A051707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = number of factorizations of (n,n) into pairs (j,k) with
 *            j>1,k>=1, counting each unordered set of factors once
 * Range:     u8 n <= 47, u16 n <= 719, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------- helper types and functions ------------- */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Structure to hold a multiplicative partition (parts sorted ascending) */
typedef struct {
    idx_t *parts;      /* array of factors (each >1) */
    size_t len;        /* number of factors */
} part_t;

/* Recursive generation of all multiplicative partitions of `rem`
   into parts >= min_f (strictly increasing to avoid duplicates). */
static void gen_partitions(idx_t rem, idx_t min_f,
                           idx_t *cur, size_t cur_len,
                           part_t **out, size_t *cnt, size_t *cap)
{
    if (rem == 1) {
        /* Finished a partition: copy current parts */
        if (*cnt == *cap) {
            *cap = (*cap ? *cap * 2 : 1);
            *out = (part_t *)realloc(*out, *cap * sizeof(part_t));
        }
        part_t newp;
        newp.len = cur_len;
        newp.parts = (idx_t *)malloc(cur_len * sizeof(idx_t));
        memcpy(newp.parts, cur, cur_len * sizeof(idx_t));
        (*out)[*cnt] = newp;
        (*cnt)++;
        return;
    }
    for (idx_t f = min_f; f <= rem; ++f) {
        if (rem % f == 0) {
            cur[cur_len] = f;
            gen_partitions(rem / f, f, cur, cur_len + 1, out, cnt, cap);
        }
    }
}

/* ---------- counting matchable pair multisets between two partitions ---------- */

static seq_t dfs_k(int k_idx, int rem_need, int *remB,
                   const int *b_counts, size_t b_len,
                   int j_idx, const int *a_counts, size_t a_len);

static seq_t dfs_j(int j_idx,
                   const int *a_counts, size_t a_len,
                   const int *b_counts, size_t b_len,
                   int *remB)
{
    if (j_idx == (int)a_len) return (seq_t)1;
    int need = a_counts[j_idx];
    return dfs_k(0, need, remB, b_counts, b_len, j_idx, a_counts, a_len);
}

static seq_t dfs_k(int k_idx, int rem_need, int *remB,
                   const int *b_counts, size_t b_len,
                   int j_idx, const int *a_counts, size_t a_len)
{
    if (k_idx == (int)b_len) {
        if (rem_need == 0)
            return dfs_j(j_idx + 1, a_counts, a_len, b_counts, b_len, remB);
        return (seq_t)0;
    }
    seq_t total = 0;
    int max_t = rem_need < remB[k_idx] ? rem_need : remB[k_idx];
    for (int t = 0; t <= max_t; ++t) {
        remB[k_idx] -= t;
        total += dfs_k(k_idx + 1, rem_need - t, remB,
                       b_counts, b_len, j_idx, a_counts, a_len);
        remB[k_idx] += t;
    }
    return total;
}

/* Count matchings for given column/row multiplicities. */
static seq_t count_match(const int *a_counts, size_t a_len,
                         const int *b_counts, size_t b_len)
{
    int *remB = (int *)malloc(b_len * sizeof(int));
    if (!remB) return (seq_t)0;  /* allocation failure – unlikely in this context */
    memcpy(remB, b_counts, b_len * sizeof(int));
    seq_t res = dfs_j(0, a_counts, a_len, b_counts, b_len, remB);
    free(remB);
    return res;
}

/* -------------------------------- page function -------------------------------- */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Generate all multiplicative partitions of n into factors >1 */
    size_t max_parts = 16;                     /* enough for n <= 10000 (log2(10000) < 14) */
    idx_t *cur = (idx_t *)malloc(max_parts * sizeof(idx_t));
    part_t *partitions = NULL;
    size_t part_cnt = 0, part_cap = 0;
    gen_partitions(n, 2, cur, 0, &partitions, &part_cnt, &part_cap);
    free(cur);

    seq_t total = 0;

    /* Main double loop over all J and K partitions */
    for (size_t idxJ = 0; idxJ < part_cnt; ++idxJ) {
        part_t Jp = partitions[idxJ];
        size_t m = Jp.len;

        /* Build multiplicity array of J (counts of equal parts) */
        size_t a_unique = 0;
        int *a_counts = (int *)malloc(m * sizeof(int));
        int last = -1;
        for (size_t i = 0; i < m; ++i) {
            int val = (int)Jp.parts[i];
            if (a_unique == 0 || val != last) {
                a_counts[a_unique++] = 1;
                last = val;
            } else {
                a_counts[a_unique - 1] += 1;
            }
        }

        for (size_t idxK = 0; idxK < part_cnt; ++idxK) {
            part_t Kp = partitions[idxK];
            size_t t = Kp.len;
            if (t > m) continue;          /* only consider K with <= m parts>1 */

            /* Build multiplicity array of K (>1 parts) */
            size_t b_unique = 0;
            int *b_counts = (int *)malloc((t + 1) * sizeof(int)); /* +1 for possible ones */
            last = -1;
            for (size_t i = 0; i < t; ++i) {
                int val = (int)Kp.parts[i];
                if (b_unique == 0 || val != last) {
                    b_counts[b_unique++] = 1;
                    last = val;
                } else {
                    b_counts[b_unique - 1] += 1;
                }
            }
            /* Add the count of ones needed to reach size m */
            size_t ones = m - t;
            if (ones > 0) {
                b_counts[b_unique++] = (int)ones;   /* ones form a new distinct value */
            }

            /* Count matchings for this pair of partitions */
            seq_t match = count_match(a_counts, a_unique, b_counts, b_unique);
            total += match;

            free(b_counts);
        }
        free(a_counts);
    }

    /* Clean up partition storage */
    for (size_t i = 0; i < part_cnt; ++i) {
        free(partitions[i].parts);
    }
    free(partitions);

    return total;
}
