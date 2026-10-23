/*
 * A033086  Numbers in which all pairs of consecutive base-8 digits differ by
 *          2.
 * https://oeis.org/A033086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 116, u32 n <= 421, u64 n <= 421,
 *            u128 n <= 421, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033086: numbers whose base‑8 digits differ by 2 pairwise.
 * This implementation generates all such numbers of increasing
 * length, sorts them, and memoises the sequence.  The function
 * a(n) returns the n‑th term (n ≥ 1).  The code works with any
 * seq_t that supports the usual arithmetic and comparison
 * operations and with idx_t=uint64_t.  It compiles as C11 or
 * C++17. */
#include <stdlib.h>
#include <stdbool.h>

/* Neighbor tables for the directed graph of admissible octal digits.
 * Each entry contains the list of digits that can follow the key digit
 * in base‑8 representation.  The root (digit 0) is forbidden as a
 * leading digit, but may appear elsewhere. */
static const int neighbors_0[1] = {2};
static const int neighbors_1[1] = {3};
static const int neighbors_2[2] = {0,4};
static const int neighbors_3[2] = {1,5};
static const int neighbors_4[2] = {2,6};
static const int neighbors_5[2] = {3,7};
static const int neighbors_6[1] = {4};
static const int neighbors_7[1] = {5};

static const int *const neighbors[8] = {
    neighbors_0, neighbors_1, neighbors_2, neighbors_3,
    neighbors_4, neighbors_5, neighbors_6, neighbors_7
};

static const int neighbor_count[8] = {1,1,2,2,2,2,1,1};

/* Global cache of generated sequence values. */
static seq_t *seq_cache      = NULL;   /* sorted list of terms */
static idx_t  seq_cache_len  = 0;      /* number of terms stored */

/* Comparator used by qsort.  Works for any seq_t providing < and >. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Recursive DFS that appends a new digit to a partially built number. */
static void dfs_generate(int last_digit, seq_t value,
                         int remaining,
                         seq_t *arr, size_t *index_ptr)
{
    if (remaining == 0) {
        arr[*index_ptr] = value;
        (*index_ptr)++;
        return;
    }
    const int *nbrs = neighbors[last_digit];
    int cnt = neighbor_count[last_digit];
    for (int i = 0; i < cnt; i++) {
        int next = nbrs[i];
        dfs_generate(next,
                     value * (seq_t)8 + (seq_t)next,
                     remaining - 1,
                     arr, index_ptr);
    }
}

/* Build the sequence up to at least the first `need` terms. */
static void build_sequence(idx_t need)
{
    if (seq_cache_len >= need) return;          /* already enough */

    /* ---- 1. Determine required maximum length ---- */
    idx_t dp_prev[8];
    idx_t dp_curr[8];
    for (int d = 0; d < 8; d++) dp_prev[d] = (d != 0) ? 1 : 0;

    idx_t cumulative = 7;         /* number of length‑1 terms */
    int length = 1;

    while (cumulative < need) {
        length++;
        for (int d = 0; d < 8; d++) {
            idx_t sum = 0;
            const int *nbrs = neighbors[d];
            int cnt = neighbor_count[d];
            for (int i = 0; i < cnt; i++) {
                int prev = nbrs[i];
                sum += dp_prev[prev];
            }
            dp_curr[d] = sum;
        }
        idx_t total_len = 0;
        for (int d = 0; d < 8; d++) total_len += dp_curr[d];
        cumulative += total_len;
        for (int d = 0; d < 8; d++) dp_prev[d] = dp_curr[d];
    }

    /* ---- 2. Allocate cache for all terms up to this length ---- */
    seq_t *temp = (seq_t *)malloc((size_t)cumulative * sizeof(seq_t));
    size_t pos = 0;

    /* ---- 3. Generate all numbers for lengths 1..length ---- */
    /* Length 1 (single digit numbers). */
    for (int d = 1; d <= 7; d++) {
        temp[pos++] = (seq_t)d;
    }

    /* Length >= 2. */
    for (int len = 2; len <= length; len++) {
        for (int start = 1; start <= 7; start++) {
            dfs_generate(start,
                         (seq_t)start,
                         len - 1,
                         temp, &pos);
        }
    }

    /* ---- 4. Sort the terms. ---- */
    qsort(temp, (size_t)cumulative, sizeof(seq_t), cmp_seq);

    /* ---- 5. Replace old cache. ---- */
    free(seq_cache);
    seq_cache      = temp;
    seq_cache_len  = cumulative;
}

/* --------------------------------------------------------------------- */
/* Public function required by the specification.                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* undefined for non‑positive indices */
    build_sequence(n);             /* ensure we have at least n terms */
    return seq_cache[n - 1];
}
