/*
 * A212219  Number of representations of n as a sum of products of distinct
 *          pairs of positive integers >=2, n = Sum_{k=1..m} i_k*j_k with
 *          2<=i_k<=j_k, i_k<i_{k+1}, j_k<j_{k+1}.
 * https://oeis.org/A212219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations of n as a sum of products of distinct pairs
   of integers >= 2, with 2 ≤ i_k ≤ j_k, i_k < i_{k+1} and j_k < j_{k+1}.  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/*-------------------------------------------------------------------------
 * Elementary integer square root (floor) – works for idx_t (≤ 2⁶⁴–1)
 *--------------------------------------------------------------------------*/
static idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/*-------------------------------------------------------------------------
 * Simple hash table for memoisation
 *--------------------------------------------------------------------------*/
struct HashMap
{
    uint64_t *keys;   /* key = (rem<<32)|(last_i<<16)|last_j ; key==0 reserved */
    seq_t   *values;  /* cached count for the key */
    uint8_t *used;    /* 0 => empty slot, 1 => occupied */
    size_t   size;    /* power of two */
};

static struct HashMap memo = {0,0,0,0};

static void hash_init(size_t size)
{
    if (memo.size) return;
    memo.size = size;
    memo.keys   = (uint64_t*)calloc(size, sizeof(uint64_t));
    memo.values = (seq_t*)calloc(size, sizeof(seq_t));
    memo.used   = (uint8_t*)calloc(size, sizeof(uint8_t));
}

static size_t hash_index(uint64_t key)
{
    return ((key ^ (key >> 32)) * 11400714819323198485ULL) & (memo.size - 1);
}

static size_t hash_find(uint64_t key, int *found)
{
    size_t idx = hash_index(key);
    size_t start = idx;
    while (memo.used[idx]) {
        if (memo.keys[idx] == key) {
            *found = 1;
            return idx;
        }
        idx = (idx + 1) & (memo.size - 1);
        if (idx == start) break;   /* table full – should not happen */
    }
    *found = 0;
    return idx;
}

static void hash_set(uint64_t key, seq_t val)
{
    int found;
    size_t pos = hash_find(key, &found);
    memo.keys[pos]   = key;
    memo.values[pos] = val;
    memo.used[pos]   = 1;
}

/*-------------------------------------------------------------------------
 * Recursive counting of representations
 *--------------------------------------------------------------------------*/
static seq_t dfs(idx_t rem, idx_t last_i, idx_t last_j)
{
    if (rem == 0) return 1;

    uint64_t key = (rem << 32) | (last_i << 16) | last_j;
    int found;
    size_t pos = hash_find(key, &found);
    if (found) return memo.values[pos];

    seq_t total = 0;

    idx_t i_start = last_i + 1;
    if (i_start < 2) i_start = 2;

    idx_t i_max = isqrt_u64(rem);
    for (idx_t i = i_start; i <= i_max; ++i) {
        idx_t j_start = (i > last_j) ? i : last_j + 1;
        idx_t j_max = rem / i;
        if (j_start > j_max) continue;
        for (idx_t j = j_start; j <= j_max; ++j) {
            total += dfs(rem - i * j, i, j);
        }
    }

    hash_set(key, total);
    return total;
}

/*-------------------------------------------------------------------------
 * Public function required by the problem
 *--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (!memo.size) hash_init(262144);   /* sufficient for n ≤ 10000 */
    return dfs(n, 0, 0);
}
