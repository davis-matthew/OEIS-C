/*
 * A122108  a(n)=number of 1's ("live" cells) at stage n of a 2-dimensional
 *          cellular automaton evolving by the rule: 1 if N+S+E+W is 1 or 2,
 *          else 0, starting from a single live cell.
 * https://oeis.org/A122108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A122108 – number of live cells after n steps in a 2‑D cellular automaton
 *  with rule: a cell becomes live if (N+S+E+W) is 1 or 2.
 *  The function simulates the automaton from a single live cell at (0,0).
 *  Complexity:   ~O(n · number_of_live_cells) ≤ O(n²).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- data structures --------------------------------------------------- */

typedef struct { int x; int y; } Point;

typedef struct {
    Point *data;
    size_t len;
    size_t cap;
} Vec;

static inline void vec_init(Vec *v) {
    v->data = NULL; v->len = 0; v->cap = 0;
}
static inline void vec_clear(Vec *v) { v->len = 0; }
static inline void vec_push(Vec *v, int x, int y) {
    if (v->len == v->cap) {
        size_t newcap = v->cap ? v->cap << 1 : 16;
        Point *tmp = (Point *)realloc(v->data, newcap * sizeof(Point));
        if (!tmp) exit(1);
        v->data = tmp; v->cap = newcap;
    }
    v->data[v->len].x = x;
    v->data[v->len].y = y;
    ++v->len;
}
static inline void vec_swap(Vec *a, Vec *b) {
    Vec tmp = *a; *a = *b; *b = tmp;
}

/* ----- hash table for neighbour counting -------------------------------- */

#define HASH_CAP  (1 << 20)            /* 1M buckets – enough for n ≤ 10000 */
#define OFFSET   0x8000U               /* 32768, guarantees positive 32‑bit parts */

static inline uint64_t hash_key(int x, int y) {
    return (((uint64_t)(uint32_t)(x + OFFSET)) << 32) | (uint32_t)(y + OFFSET);
}
static inline void key_decode(uint64_t key, int *x, int *y) {
    *x = (int)((key >> 32) & 0xFFFFFFFFu) - OFFSET;
    *y = (int)(key & 0xFFFFFFFFu) - OFFSET;
}

typedef struct {
    size_t mask;
    uint64_t *keys;   /* bucket keys */
    uint8_t  *cnt;    /* neighbour count (capped 0‑3) */
    uint8_t  *used;   /* occupied flag */
    size_t *indices;  /* list of used bucket indices */
    size_t indices_len;
    size_t indices_cap;
} HashTable;

static HashTable ht = {0};
static int ht_inited = 0;

static void ht_init(void) {
    if (ht_inited) return;
    size_t cap = HASH_CAP;
    ht.mask = cap - 1;
    ht.keys = (uint64_t *)calloc(cap, sizeof(uint64_t));
    ht.cnt  = (uint8_t *)calloc(cap, sizeof(uint8_t));
    ht.used = (uint8_t *)calloc(cap, sizeof(uint8_t));
    if (!ht.keys || !ht.cnt || !ht.used) exit(1);
    ht.indices = NULL; ht.indices_len = 0; ht.indices_cap = 0;
    ht_inited = 1;
}

static void ht_insert(uint64_t key) {
    size_t idx = key & ht.mask;
    while (ht.used[idx]) {
        if (ht.keys[idx] == key) break;
        idx = (idx + 1) & ht.mask;
    }
    if (!ht.used[idx]) {
        /* new entry */
        ht.used[idx] = 1;
        ht.keys[idx] = key;
        ht.cnt[idx]  = 0;
        /* store index for later reset */
        if (ht.indices_len == ht.indices_cap) {
            ht.indices_cap = ht.indices_cap ? ht.indices_cap << 1 : 1024;
            size_t *tmp = (size_t *)realloc(ht.indices, ht.indices_cap * sizeof(size_t));
            if (!tmp) exit(1);
            ht.indices = tmp;
        }
        ht.indices[ht.indices_len++] = idx;
    }
    if (ht.cnt[idx] < 3) ++ht.cnt[idx];
}

/* ----- main function ----------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    if (!ht_inited) ht_init();

    static Vec cur, next;
    static int vec_init_done = 0;
    if (!vec_init_done) {
        vec_init(&cur);
        vec_init(&next);
        vec_init_done = 1;
    }

    vec_clear(&cur);
    vec_push(&cur, 0, 0);   /* start with single live cell */

    const int dx[4] = { 1, -1, 0, 0 };
    const int dy[4] = { 0, 0, 1, -1 };

    for (idx_t step = 1; step <= n - 1; ++step) {
        vec_clear(&next);

        /* count neighbours of each live cell */
        for (size_t i = 0; i < cur.len; ++i) {
            int cx = cur.data[i].x;
            int cy = cur.data[i].y;
            for (int d = 0; d < 4; ++d)
                ht_insert(hash_key(cx + dx[d], cy + dy[d]));
        }

        /* build next state from counts */
        for (size_t i = 0; i < ht.indices_len; ++i) {
            size_t idx = ht.indices[i];
            uint8_t c = ht.cnt[idx];
            if (c == 1 || c == 2) {
                int nx, ny;
                key_decode(ht.keys[idx], &nx, &ny);
                vec_push(&next, nx, ny);
            }
            ht.used[idx] = 0;
            ht.cnt[idx]  = 0;
        }
        ht.indices_len = 0;

        vec_swap(&cur, &next);
    }

    return (seq_t)cur.len;
}
