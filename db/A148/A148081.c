/*
 * A148081  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 0,
 *          -1), (0, -1, 1), (0, 1, 0), (1, 0, -1)}.
 * https://oeis.org/A148081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of walks of length n in N^3 starting at (0,0,0)
 *            using steps {(-1,-1,0), (-1,0,-1), (0,-1,1), (0,1,0), (1,0,-1)}
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------
   Dependencies: the build supplies the types
       seq_t  – unsigned integer or arbitrary‑precision integer
       idx_t  – unsigned integer (uint64_t or similar)
   No additional headers beyond the standard C library are required.
   -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef HASH_CAPACITY
#define HASH_CAPACITY (1U << 21)   /* ~2 million buckets, > state volume for n≤10000 */
#endif

/* Forward‑declarations – the types are supplied by the build. */
typedef uint64_t idx_t; /* placeholder */

/* --------------------------------------------------------------------
   Static storage for the hash table and auxiliary arrays.
   -------------------------------------------------------------------- */
static uint64_t *hash_key = NULL;
static seq_t   *hash_val = NULL;
static unsigned char *hash_used = NULL;
static size_t hash_cap = 0, hash_mask = 0;

static size_t *used_idx = NULL;   /* indices of buckets that were set in the current step */
static size_t  used_cap = 0;

/* Structure representing a state: (x,y,z) together with the number of walks. */
typedef struct {
    int x, y, z;
    seq_t cnt;
} State;

/* Arrays for the frontier of the current and next step. */
static State *curr = NULL;
static State *next = NULL;

/* ---------------------------------------------------------------
   Utility: hash function for 64‑bit keys.
   --------------------------------------------------------------- */
static inline size_t hash_key64(uint64_t key) {
    size_t h = (size_t)key;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return h & hash_mask;
}

/* ---------------------------------------------------------------
   Main function: compute a(n).
   --------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* ------------------------------------------------------------
       One‑time allocation of static tables.
       ------------------------------------------------------------ */
    if (!hash_key) {
        hash_cap  = HASH_CAPACITY;
        hash_mask = hash_cap - 1;
#ifdef __cplusplus
        hash_key  = new uint64_t[hash_cap];
        hash_val  = new seq_t[hash_cap];
        hash_used = new unsigned char[hash_cap];
#else
        hash_key  = (uint64_t*)malloc(hash_cap * sizeof(uint64_t));
        hash_val  = (seq_t*)malloc(hash_cap * sizeof(seq_t));
        hash_used = (unsigned char*)malloc(hash_cap * sizeof(unsigned char));
#endif
        memset(hash_used, 0, hash_cap);
    }

    if (!used_idx) {
        used_cap = hash_cap;
#ifdef __cplusplus
        used_idx = new size_t[used_cap];
#else
        used_idx = (size_t*)malloc(used_cap * sizeof(size_t));
#endif
    }

    if (!curr) {
#ifdef __cplusplus
        curr = new State[hash_cap];
        next = new State[hash_cap];
#else
        curr = (State*)malloc(hash_cap * sizeof(State));
        next = (State*)malloc(hash_cap * sizeof(State));
#endif
    }

    /* ------------------------------------------------------------
       Initialize frontier: only the origin is reachable.
       ------------------------------------------------------------ */
    size_t curr_len = 1;
    curr[0].x = 0; curr[0].y = 0; curr[0].z = 0;
    curr[0].cnt = (seq_t)1U;

    /* ------------------------------------------------------------
       Iterate over steps.
       ------------------------------------------------------------ */
    for (idx_t step = 0; step < n; ++step) {
        size_t used_cnt = 0;

        /* For every state in the current frontier generate the 5 possible moves. */
        for (size_t i = 0; i < curr_len; ++i) {
            int x = curr[i].x;
            int y = curr[i].y;
            int z = curr[i].z;
            seq_t cnt = curr[i].cnt;

            /* Move 1: (-1,-1,0) */
            if (x > 0 && y > 0) {
                int nx = x - 1, ny = y - 1, nz = z;
                uint64_t key = ((uint64_t)nx << 40) | ((uint64_t)ny << 20) | (uint64_t)nz;
                size_t h = hash_key64(key);
                if (!hash_used[h]) {
                    hash_used[h] = 1;
                    hash_key[h] = key;
                    hash_val[h] = cnt;
                    used_idx[used_cnt++] = h;
                } else {
                    hash_val[h] += cnt;
                }
            }

            /* Move 2: (-1,0,-1) */
            if (x > 0 && z > 0) {
                int nx = x - 1, ny = y, nz = z - 1;
                uint64_t key = ((uint64_t)nx << 40) | ((uint64_t)ny << 20) | (uint64_t)nz;
                size_t h = hash_key64(key);
                if (!hash_used[h]) {
                    hash_used[h] = 1;
                    hash_key[h] = key;
                    hash_val[h] = cnt;
                    used_idx[used_cnt++] = h;
                } else {
                    hash_val[h] += cnt;
                }
            }

            /* Move 3: (0,-1,1) */
            if (y > 0) {
                int nx = x, ny = y - 1, nz = z + 1;
                uint64_t key = ((uint64_t)nx << 40) | ((uint64_t)ny << 20) | (uint64_t)nz;
                size_t h = hash_key64(key);
                if (!hash_used[h]) {
                    hash_used[h] = 1;
                    hash_key[h] = key;
                    hash_val[h] = cnt;
                    used_idx[used_cnt++] = h;
                } else {
                    hash_val[h] += cnt;
                }
            }

            /* Move 4: (0,1,0) */
            {
                int nx = x, ny = y + 1, nz = z;
                uint64_t key = ((uint64_t)nx << 40) | ((uint64_t)ny << 20) | (uint64_t)nz;
                size_t h = hash_key64(key);
                if (!hash_used[h]) {
                    hash_used[h] = 1;
                    hash_key[h] = key;
                    hash_val[h] = cnt;
                    used_idx[used_cnt++] = h;
                } else {
                    hash_val[h] += cnt;
                }
            }

            /* Move 5: (1,0,-1) */
            if (z > 0) {
                int nx = x + 1, ny = y, nz = z - 1;
                uint64_t key = ((uint64_t)nx << 40) | ((uint64_t)ny << 20) | (uint64_t)nz;
                size_t h = hash_key64(key);
                if (!hash_used[h]) {
                    hash_used[h] = 1;
                    hash_key[h] = key;
                    hash_val[h] = cnt;
                    used_idx[used_cnt++] = h;
                } else {
                    hash_val[h] += cnt;
                }
            }
        }

        /* ----------------------------------------------------------
           Build the frontier for the next step from the hash table.
           ---------------------------------------------------------- */
        size_t next_len = used_cnt;
        for (size_t j = 0; j < used_cnt; ++j) {
            size_t h = used_idx[j];
            uint64_t key = hash_key[h];
            int nx = (int)(key >> 40);
            int ny = (int)((key >> 20) & ((1ULL << 20) - 1));
            int nz = (int)(key & ((1ULL << 20) - 1));

            next[j].x = nx;
            next[j].y = ny;
            next[j].z = nz;
            next[j].cnt = hash_val[h];

            /* Reset the hash bucket for the next iteration. */
            hash_used[h] = 0;
        }

        /* Swap the frontiers for the next iteration. */
        State *tmp = curr;
        curr = next;
        next = tmp;
        curr_len = next_len;
    }

    /* ------------------------------------------------------------
       Sum up all counts in the final frontier.
       ------------------------------------------------------------ */
    seq_t result = 0;
    for (size_t i = 0; i < curr_len; ++i) {
        result += curr[i].cnt;
    }
    return result;
}
