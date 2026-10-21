/*
 * A222118  Number of terms in Collatz (3x+1) trajectory of n that did not
 *          appear in previous trajectories.
 * https://oeis.org/A222118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006577(n) - A221956(n) + 1
 * Range:     u8 n <= 26, u16 n <= 9188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of terms in Collatz trajectory of n that did not appear in previous trajectories */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_to_str.h"

/* ---------- Hash table for visited sequence terms ---------- */

static seq_t *hash_keys = NULL;
static uint8_t *hash_used = NULL;
static size_t hash_cap = 0;
static size_t hash_used_count = 0;

/* hash function using bit‑shifts, works for 64‑bit and larger seq_t types */
static size_t seq_hash(seq_t x) {
    size_t h = 0;
    if (sizeof(seq_t) <= 8) {          /* 64‑bit or smaller */
        uint64_t v = (uint64_t)x;
        while (v != 0) {
            h = h * 1315423911u + (uint32_t)(v & 0xFFFFFFFFu);
            v >>= 32;
        }
    } else {                           /* > 64 bits, e.g. __int128 or arbitrary‑precision */
        seq_t t = x;
        while (t != (seq_t)0) {
            uint64_t part = (uint64_t)(t & ((seq_t)0xFFFFFFFFFFFFFFFFULL));
            h = h * 1315423911u + part;
            t = t >> 64;
        }
    }
    return h;
}

/* Rehash to new capacity (must be power of two, ≥8) */
static void hash_rehash(size_t new_cap) {
    seq_t *old_keys = hash_keys;
    uint8_t *old_used = hash_used;
    size_t old_cap = hash_cap;

    hash_keys = (seq_t *)malloc(new_cap * sizeof(seq_t));
    hash_used = (uint8_t *)calloc(new_cap, 1);
    hash_cap = new_cap;
    hash_used_count = 0;

    for (size_t i = 0; i < old_cap; ++i) {
        if (old_used[i]) {
            /* Insert without duplicate check; old data guaranteed unique */
            size_t h = seq_hash(old_keys[i]);
            size_t idx = h & (hash_cap - 1);
            while (hash_used[idx]) idx = (idx + 1) & (hash_cap - 1);
            hash_used[idx] = 1;
            hash_keys[idx] = old_keys[i];
            hash_used_count++;
        }
    }

    free(old_keys);
    free(old_used);
}

/* Tries to insert x. Returns true if x was not present before. */
static bool hash_try_insert(seq_t x) {
    if (hash_cap == 0) hash_rehash(8);
    if ((hash_used_count * 2) >= hash_cap) hash_rehash(hash_cap * 2);

    size_t h = seq_hash(x);
    size_t idx = h & (hash_cap - 1);
    while (hash_used[idx]) {
        if (hash_keys[idx] == x) return false;   /* already present */
        idx = (idx + 1) & (hash_cap - 1);
    }
    hash_used[idx] = 1;
    hash_keys[idx] = x;
    hash_used_count++;
    return true;
}

/* ---------- Memoisation of a(n) values ---------- */

static seq_t *memo = NULL;
static idx_t memo_len = 0;      /* number of elements currently stored */
static size_t memo_cap = 0;     /* current capacity of memo array */

/* Main function to compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety guard, though n>=1 by definition */

    /* Grow memo array if necessary */
    if (n > memo_len) {
        size_t new_cap = memo_cap ? memo_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo_cap = new_cap;

        /* Compute a(i) for all new i */
        for (idx_t i = memo_len + 1; i <= n; ++i) {
            seq_t start = (seq_t)i;
            seq_t count = 0;
            seq_t current = start;

            while (1) {
                if (hash_try_insert(current)) count++;  /* new term */
                if (current == (seq_t)1) break;         /* reached 1, stop */
                if ((current & (seq_t)1) == 0) {
                    current = current / (seq_t)2;
                } else {
                    current = current * (seq_t)3 + (seq_t)1;
                }
            }
            memo[i - 1] = count;
        }
        memo_len = n;
    }
    return memo[n - 1];
}
