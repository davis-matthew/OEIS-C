/*
 * A008320  Smallest number that is not the sum of squares of two distinct
 *          earlier terms.
 * https://oeis.org/A008320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1170, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008320: smallest number that is not the sum of squares of two distinct earlier terms (computed by greedy search). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *seq_arr  = NULL;
static size_t seq_cap  = 0;
static size_t seq_size = 0;
static seq_t *sq_arr   = NULL;

static seq_t *hash_keys = NULL;
static size_t *hash_vals = NULL;
static size_t hash_cap = 0;
static size_t hash_size = 0;
static bool initialized = false;

static void ensure_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap << 1 : 1;
    while (cap < need) cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, cap * sizeof(seq_t));
    sq_arr  = (seq_t *)realloc(sq_arr,  cap * sizeof(seq_t));
    seq_cap = cap;
}

static void hash_grow(void)
{
    size_t new_cap = hash_cap ? hash_cap << 1 : 8;
    seq_t *new_keys = (seq_t *)calloc(new_cap, sizeof(seq_t));
    size_t *new_vals = (size_t *)calloc(new_cap, sizeof(size_t));
    if (!new_keys || !new_vals) exit(EXIT_FAILURE);
    for (size_t i = 0; i < hash_cap; ++i) {
        if (hash_keys[i] != (seq_t)0) {
            seq_t key = hash_keys[i];
            size_t val = hash_vals[i];
            size_t pos = (size_t)((uint64_t)key & (new_cap - 1));
            while (new_keys[pos] != (seq_t)0) pos = (pos + 1) & (new_cap - 1);
            new_keys[pos] = key;
            new_vals[pos] = val;
        }
    }
    free(hash_keys);
    free(hash_vals);
    hash_keys = new_keys;
    hash_vals = new_vals;
    hash_cap  = new_cap;
}

static void hash_insert(seq_t key, size_t val)
{
    if (hash_cap == 0) {
        hash_cap = 8;
        hash_keys = (seq_t *)calloc(8, sizeof(seq_t));
        hash_vals = (size_t *)calloc(8, sizeof(size_t));
    }
    if (hash_size * 2 >= hash_cap) hash_grow();
    size_t pos = (size_t)((uint64_t)key & (hash_cap - 1));
    while (hash_keys[pos] != (seq_t)0) pos = (pos + 1) & (hash_cap - 1);
    hash_keys[pos] = key;
    hash_vals[pos] = val;
    ++hash_size;
}

static size_t hash_lookup(seq_t key)
{
    if (hash_cap == 0) return SIZE_MAX;
    size_t pos = (size_t)((uint64_t)key & (hash_cap - 1));
    while (hash_keys[pos] != (seq_t)0) {
        if (hash_keys[pos] == key) return hash_vals[pos];
        pos = (pos + 1) & (hash_cap - 1);
    }
    return SIZE_MAX;
}

static bool is_representable(seq_t x)
{
    for (size_t i = 0; i < seq_size; ++i) {
        seq_t sq = sq_arr[i];
        if (sq >= x) continue;
        seq_t rem = x - sq;
        size_t j = hash_lookup(rem);
        if (j != SIZE_MAX && j != i) return true;
    }
    return false;
}

static void init(void)
{
    ensure_seq(8);
    seq_arr[0] = (seq_t)1;
    sq_arr[0] = seq_arr[0] * seq_arr[0];
    seq_size = 1;

    hash_cap = 8;
    hash_keys = (seq_t *)calloc(hash_cap, sizeof(seq_t));
    hash_vals = (size_t *)calloc(hash_cap, sizeof(size_t));
    hash_insert(sq_arr[0], 0);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!initialized) init();

    while (seq_size < (size_t)n) {
        seq_t cand = seq_arr[seq_size - 1] + (seq_t)1;
        while (is_representable(cand)) cand += (seq_t)1;
        ensure_seq(seq_size + 1);
        seq_arr[seq_size] = cand;
        sq_arr[seq_size] = cand * cand;
        hash_insert(sq_arr[seq_size], seq_size);
        ++seq_size;
    }
    return seq_arr[n - 1];
}
