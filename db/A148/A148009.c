/*
 * A148009  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 0,
 *          -1), (0, -1, 1), (0, 1, 0), (1, -1, -1)}.
 * https://oeis.org/A148009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 22, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   hash_xyz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A148009: number of walks of length n in the first octant
   of Z^3 using the step set
   {(-1,-1,0), (-1,0,-1), (0,-1,1), (0,1,0), (1,-1,-1)}.
   The implementation uses a memoised dynamic programming approach.
   For each step all currently reachable states are expanded to the next
   step using the allowed steps, keeping only states with non‑negative
   coordinates.  The number of walks of length n is the sum of the counts
   of all states after n steps.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/hash_xyz.h"

/* ---------- Hash table for states ---------- */

typedef struct {
    int x, y, z;     /* coordinates */
    seq_t val;       /* number of walks reaching this state */
    uint8_t used;    /* 0 = empty, 1 = occupied */
} Entry;

typedef struct {
    Entry *tbl;          /* array of entries */
    size_t capacity;     /* power of two */
    size_t mask;         /* capacity-1, for modulo */
    size_t size;         /* number of occupied slots */
    size_t *used_idx;    /* indices of occupied slots (for fast clear) */
    size_t used_count;   /* number of occupied slots */
} HashTable;

static void ht_init(HashTable *ht, size_t init_cap)
{
    size_t cap = 1;
    while (cap < init_cap) cap <<= 1;
    ht->capacity = cap;
    ht->mask     = cap - 1;
    ht->size     = 0;
    ht->used_count = 0;
    ht->tbl = (Entry *)calloc(cap, sizeof(Entry));
    ht->used_idx = (size_t *)calloc(cap, sizeof(size_t));
}

static void ht_free(HashTable *ht)
{
    free(ht->tbl);
    free(ht->used_idx);
}

static void ht_clear(HashTable *ht)
{
    for (size_t i = 0; i < ht->used_count; ++i) {
        size_t idx = ht->used_idx[i];
        ht->tbl[idx].used = 0;
    }
    ht->size = 0;
    ht->used_count = 0;
}

/* Rehash to double capacity */
static void ht_rehash(HashTable *ht)
{
    size_t old_cap   = ht->capacity;
    Entry *old_tbl   = ht->tbl;
    size_t *old_idx  = ht->used_idx;

    size_t new_cap   = old_cap << 1;
    ht->capacity     = new_cap;
    ht->mask         = new_cap - 1;

    ht->tbl          = (Entry *)calloc(new_cap, sizeof(Entry));
    ht->used_idx     = (size_t *)calloc(new_cap, sizeof(size_t));
    ht->size         = 0;
    ht->used_count   = 0;

    for (size_t i = 0; i < old_cap; ++i) {
        if (old_tbl[i].used) {
            /* reinsert existing entry */
            /* use the same insertion code without rehashing again */
            size_t h = hash_xyz(old_tbl[i].x, old_tbl[i].y, old_tbl[i].z);
            size_t idx = h & ht->mask;
            while (ht->tbl[idx].used) idx = (idx + 1) & ht->mask;
            ht->tbl[idx] = old_tbl[i];
            ht->tbl[idx].used = 1;
            ht->used_idx[ht->used_count++] = idx;
            ht->size++;
        }
    }

    free(old_tbl);
    free(old_idx);
}

static void ht_insert_add(HashTable *ht, int x, int y, int z, seq_t val)
{
    /* Resize if load factor exceeds 0.75 */
    if (ht->size > (ht->capacity * 3) / 4) {
        ht_rehash(ht);
    }

    size_t h = hash_xyz(x, y, z);
    size_t idx = h & ht->mask;
    while (ht->tbl[idx].used) {
        if (ht->tbl[idx].x == x && ht->tbl[idx].y == y && ht->tbl[idx].z == z) {
            ht->tbl[idx].val += val;
            return;
        }
        idx = (idx + 1) & ht->mask;
    }
    /* slot is free */
    ht->tbl[idx].x   = x;
    ht->tbl[idx].y   = y;
    ht->tbl[idx].z   = z;
    ht->tbl[idx].val = val;
    ht->tbl[idx].used = 1;
    ht->used_idx[ht->used_count++] = idx;
    ht->size++;
}

/* ---------- Sequence function ---------- */

seq_t a(idx_t n)
{
    /* static state to keep memoisation across calls */
    static bool          initialized = false;
    static seq_t        *memo = NULL;
    static idx_t         memo_size = 0;   /* number of terms in memo */
    static HashTable     cur, nxt;        /* tables for current / next step */
    static const int steps[5][3] = {
        {-1, -1,  0},
        {-1,  0, -1},
        { 0, -1,  1},
        { 0,  1,  0},
        { 1, -1, -1}
    };

    if (!initialized) {
        /* first call – initialise structures */
        ht_init(&cur, 1024);
        ht_init(&nxt, 1024);

        memo = (seq_t *)malloc(sizeof(seq_t));
        memo[0] = (seq_t)1;   /* a(0) = 1 */
        memo_size = 1;

        ht_insert_add(&cur, 0, 0, 0, (seq_t)1);  /* state (0,0,0) has 1 walk */

        initialized = true;
    }

    /* Compute up to requested n if needed */
    while (n >= memo_size) {
        /* generate nxt from cur */
        for (size_t i = 0; i < cur.used_count; ++i) {
            Entry *e = &cur.tbl[cur.used_idx[i]];
            for (int k = 0; k < 5; ++k) {
                int nx = e->x + steps[k][0];
                int ny = e->y + steps[k][1];
                int nz = e->z + steps[k][2];
                if (nx >= 0 && ny >= 0 && nz >= 0) {
                    ht_insert_add(&nxt, nx, ny, nz, e->val);
                }
            }
        }

        /* sum counts in nxt to obtain a(memo_size) */
        seq_t total = 0;
        for (size_t i = 0; i < nxt.used_count; ++i) {
            total += nxt.tbl[nxt.used_idx[i]].val;
        }

        /* append to memo */
        memo_size++;
        memo = (seq_t *)realloc(memo, memo_size * sizeof(seq_t));
        memo[memo_size - 1] = total;

        /* prepare for next iteration */
        HashTable tmp = cur;
        cur = nxt;
        nxt = tmp;

        ht_clear(&nxt);   /* clear the old table for reuse */
    }

    return memo[n];
}
