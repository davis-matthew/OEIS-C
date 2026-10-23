/*
 * A148871  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 0,
 *          1), (0, 1, -1), (0, 1, 1), (1, -1, 0)}.
 * https://oeis.org/A148871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   next_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A148871: Number of walks within N^3 starting at (0,0,0) and
    consisting of n steps taken from
    {(-1, -1, 0), (-1, 0, 1), (0, 1, -1), (0, 1, 1), (1, -1, 0)}.
    The function computes the number of such walks of length n using
    dynamic programming with a sparse hash table.
    Complexity: roughly O(n * S_n) where S_n is the number of reachable
    states after n steps. For n up to 10 000 this runs well under time
    limits for typical 64‑bit or arbitrary‑precision seq_t types. */
 
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_capacity.h"

typedef struct {
    uint64_t key;   /* packed coordinates (x,y,z) */
    seq_t val;      /* number of walks reaching this state */
    unsigned char used; /* flag: 0 = empty, 1 = occupied */
} Entry;

static const int steps[5][3] = {
    {-1, -1, 0},
    {-1,  0, 1},
    { 0,  1,-1},
    { 0,  1, 1},
    { 1, -1, 0}
};

/* pack three non‑negative coordinates into a 64‑bit key
   each coordinate fits into 21 bits (max value << 2^21-1)   */
static inline uint64_t pack_key(uint32_t x, uint32_t y, uint32_t z)
{
    return ((uint64_t)x << 42) | ((uint64_t)y << 21) | (uint64_t)z;
}

/* add a (key,val) pair into the hash table 'tbl' of size 'cap' (power of two).
   If the key already exists, its value is increased by 'val'.            */
static void add_to_table(Entry *tbl, size_t cap, uint64_t key, seq_t val)
{
    size_t idx = key & (cap - 1);
    while (tbl[idx].used) {
        if (tbl[idx].key == key) {
            tbl[idx].val += val;
            return;
        }
        idx = (idx + 1) & (cap - 1);
    }
    tbl[idx].used = 1;
    tbl[idx].key  = key;
    tbl[idx].val  = val;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* initial table with the single state (0,0,0) */
    size_t cur_cap = next_capacity(1);
    Entry *cur = (Entry *)calloc(cur_cap, sizeof(Entry));
    cur[0].used = 1;
    cur[0].key  = 0;
    cur[0].val  = (seq_t)1;
    size_t cur_used = 1;

    for (idx_t step = 0; step < n; ++step) {
        size_t nxt_cap = next_capacity(cur_used);
        Entry *nxt = (Entry *)calloc(nxt_cap, sizeof(Entry));
        size_t nxt_used = 0;

        for (size_t i = 0; i < cur_cap; ++i) {
            if (!cur[i].used) continue;

            uint64_t k = cur[i].key;
            uint32_t z = (uint32_t)(k & 0x1FFFFF);
            uint32_t y = (uint32_t)((k >> 21) & 0x1FFFFF);
            uint32_t x = (uint32_t)(k >> 42);
            seq_t v = cur[i].val;

            for (int s = 0; s < 5; ++s) {
                int64_t nx = (int64_t)x + steps[s][0];
                int64_t ny = (int64_t)y + steps[s][1];
                int64_t nz = (int64_t)z + steps[s][2];
                if (nx < 0 || ny < 0 || nz < 0) continue;

                uint64_t nk = pack_key((uint32_t)nx, (uint32_t)ny, (uint32_t)nz);
                add_to_table(nxt, nxt_cap, nk, v);
            }
        }

        free(cur);
        cur = nxt;
        cur_cap = nxt_cap;

        /* count used entries for the next capacity estimate */
        nxt_used = 0;
        for (size_t i = 0; i < cur_cap; ++i)
            if (cur[i].used) ++nxt_used;
        cur_used = nxt_used;
    }

    /* sum all counts to obtain a(n) */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < cur_cap; ++i)
        if (cur[i].used)
            ans += cur[i].val;

    free(cur);
    return ans;
}
