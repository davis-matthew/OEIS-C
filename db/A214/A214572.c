/*
 * A214572  The Matula-Goebel numbers of the rooted trees having 8 vertices.
 * https://oeis.org/A214572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest Matula-Goebel number among all rooted trees
 *            with 8 vertices. The numbers are computed by generating all
 *            rooted trees of size 8 and sorting the resulting Matula values
 * Range:     u8 none, u16 n <= 115, u32 n <= 115, u64 n <= 115, u128 n <= 115,
 *            big any n
 * Helpers:   cmp_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/cmp_seq_t.h"

/* Maximum number of rooted trees with 8 vertices (A000081(8)). */
#define MAX_TREES 115

/* Storage for Matula values of all trees of size 1..8. */
static seq_t tree_vals[9][MAX_TREES];
static int tree_cnt[9] = {0};

/* Simple in‑place insertion sort using cmp_seq_t. */
static void sort_seq_arr(seq_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && cmp_seq_t(&key, &arr[j-1]) < 0) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = key;
    }
}

/* Recursive combination of subtrees for a partition.  The array 'chosen'
   keeps the previously selected child index to avoid ordering duplicates
   when consecutive parts are equal. */
static void combine_with_index(int idx, int depth, const int parts[],
                               seq_t prod, int target_n, int *chosen)
{
    if (idx == depth) {
        tree_vals[target_n][tree_cnt[target_n]++] = prod;
        return;
    }

    int sz = parts[idx];
    /* Ensure subtrees of this size are already built. */
    if (tree_cnt[sz] == 0) {
        /* Recursive build for smaller sizes. */
        for (int s = 1; s < sz; ++s) {
            if (tree_cnt[s] == 0) {
                /* Build smaller subtrees lazily. */
                /* The build_n function will be called indirectly
                   when needed below. */
            }
        }
    }
    int start = 0;
    if (idx > 0 && parts[idx] == parts[idx-1]) {
        start = chosen[idx-1];   /* enforce non‑decreasing indices */
    }
    for (int i = start; i < tree_cnt[sz]; ++i) {
        chosen[idx] = i;
        seq_t sub = tree_vals[sz][i];
        seq_t prime = get_prime((idx_t)sub);
        combine_with_index(idx+1, depth, parts,
                           prod * prime, target_n, chosen);
    }
}

/* Launch combination for a complete partition. */
static void combine_partition(int depth, const int parts[], int target_n)
{
    int chosen[8] = {0};
    combine_with_index(0, depth, parts, (seq_t)1, target_n, chosen);
}

/* Generate all partitions of 'rem' into non‑decreasing parts. */
static void partition_rec(int rem, int min_part, int depth,
                          int parts[], int target_n)
{
    if (rem == 0) {
        combine_partition(depth, parts, target_n);
        return;
    }
    for (int v = min_part; v <= rem; ++v) {
        parts[depth] = v;
        partition_rec(rem - v, v, depth + 1, parts, target_n);
    }
}

/* Recursive construction of all Matula values for trees of size 'n'. */
static void build_n(int n)
{
    if (tree_cnt[n] != 0) return;   /* already built */

    if (n == 1) {
        tree_vals[1][0] = (seq_t)1;
        tree_cnt[1] = 1;
        return;
    }

    int parts[8];
    partition_rec(n - 1, 1, 0, parts, n);
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Lazy initialization of the sequence for size 8. */
    static bool init = false;
    static seq_t sorted[MAX_TREES];
    static size_t seq_len = 0;

    if (!init) {
        /* Build all Matula values for trees with 8 vertices. */
        for (int sz = 1; sz <= 8; ++sz)
            build_n(sz);

        seq_len = (size_t)tree_cnt[8];
        for (size_t i = 0; i < seq_len; ++i)
            sorted[i] = tree_vals[8][i];
        sort_seq_arr(sorted, seq_len);
        init = true;
    }

    if (n >= 1 && (idx_t)n <= (idx_t)seq_len)
        return sorted[(size_t)n - 1];

    /* Undefined for indices beyond the sequence. */
    return (seq_t)0;
}
