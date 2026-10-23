/*
 * A163006  Number of n X 6 binary arrays with all 1's connected and a path of
 *          1's from upper left corner to lower right corner.
 * https://oeis.org/A163006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A163006: number of n×6 binary arrays with all 1's connected
 * and a path of 1's from upper left corner (1,1) to lower right corner (n,6).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static inline unsigned find_root(unsigned v, unsigned *p)
{
    while (p[v] != v) {
        p[v] = p[p[v]];
        v = p[v];
    }
    return v;
}

static inline void union_sets(unsigned a, unsigned b, unsigned *p)
{
    a = find_root(a, p);
    b = find_root(b, p);
    if (a != b) p[b] = a;
}

/* encode component labels (-1 => unused) into 24‑bit key */
static inline void encode_state(const int *lab, uint32_t *key)
{
    uint32_t k = 0;
    for (int j = 0; j < 6; ++j) {
        uint32_t v = (lab[j] < 0) ? 0xFu : (unsigned)lab[j];
        k = (k << 4) | v;
    }
    *key = k;
}

/* reverse operation */
static inline void decode_state(uint32_t key, int *lab)
{
    for (int j = 5; j >= 0; --j) {
        unsigned v = key & 0xFu;
        key >>= 4;
        lab[j] = (int)((v == 0xFu) ? -1 : v);
    }
}

/* ------------------------------------------------------------ */
/*  Simple hash map (open addressing) for <key, val> pairs      */
/* ------------------------------------------------------------ */
#define HASH_SIZE 16384
#define EMPTY_KEY 0xFFFFFFFFu
typedef struct { uint32_t key; seq_t val; } HashEntry;
static HashEntry hash_tbl[HASH_SIZE];
static uint32_t used_idx[HASH_SIZE];
static int used_cnt;

static inline void hash_init(void)
{
    for (int i = 0; i < HASH_SIZE; ++i) hash_tbl[i].key = EMPTY_KEY;
}

static inline void hash_clear(void)
{
    for (int i = 0; i < used_cnt; ++i) {
        uint32_t idx = used_idx[i];
        hash_tbl[idx].key = EMPTY_KEY;
    }
    used_cnt = 0;
}

static inline void hash_insert(uint32_t key, seq_t val)
{
    unsigned h = key & (HASH_SIZE - 1);
    while (hash_tbl[h].key != EMPTY_KEY && hash_tbl[h].key != key)
        h = (h + 1) & (HASH_SIZE - 1);

    if (hash_tbl[h].key == EMPTY_KEY) {
        hash_tbl[h].key = key;
        hash_tbl[h].val = val;
        used_idx[used_cnt++] = h;
    } else {
        hash_tbl[h].val += val;
    }
}

/* ------------------------------------------------------------ */
typedef struct { uint32_t key; seq_t val; } State;
#define MAX_STATES 2048
static State curr[MAX_STATES];
static int curr_len;

seq_t a(idx_t n)
{
    const int W = 6;

    /* ---------- build initial row (row 1) ---------- */
    curr_len = 0;
    for (int m = 1; m < 64; ++m) {           /* UL cell (col 0) must be 1 */
        if ((m & 1) == 0) continue;

        unsigned p[12];
        for (int i = 0; i < 12; ++i) p[i] = i;
        unsigned col2uf[6];
        unsigned uf_next = 1;                 /* 0 used for UL */
        for (int j = 0; j < W; ++j) {
            if (m & (1 << j))
                col2uf[j] = (j == 0) ? 0 : uf_next++;
            else
                col2uf[j] = 0xFFFFFFFFu;
        }
        /* horizontal unions */
        for (int j = 1; j < W; ++j)
            if ((m & (1 << j)) && (m & (1 << (j - 1))))
                union_sets(col2uf[j - 1], col2uf[j], p);

        /* canonical labeling: root of 0 -> label 0 */
        int root_to_lbl[12];
        for (int i = 0; i < 12; ++i) root_to_lbl[i] = -1;
        root_to_lbl[find_root(0, p)] = 0;
        int next_lbl = 1;
        int lab[6];
        for (int j = 0; j < W; ++j) {
            if (m & (1 << j)) {
                unsigned r = find_root(col2uf[j], p);
                if (root_to_lbl[r] == -1) root_to_lbl[r] = next_lbl++;
                lab[j] = root_to_lbl[r];
            } else
                lab[j] = -1;
        }
        uint32_t key; encode_state(lab, &key);
        curr[curr_len].key = key;
        curr[curr_len++].val = 1;   /* each mask counted once initially */
    }

    /* ---------- DP over remaining rows ---------- */
    hash_init();
    for (idx_t row = 2; row <= n; ++row) {
        hash_clear();
        for (int i = 0; i < curr_len; ++i) {
            const State *st = &curr[i];
            seq_t ways = st->val;
            int prev_lab[6];
            decode_state(st->key, prev_lab);

            /* gather info on previous component ids */
            int seen[6] = {0};
            bool pers[6] = {0};
            int max_old = -1;
            for (int j = 0; j < W; ++j) {
                int id = prev_lab[j];
                if (id >= 0) {
                    seen[id] = 1;
                    if (id > max_old) max_old = id;
                }
            }

            for (int m = 0; m < 64; ++m) {
                /* reset persistence flags */
                for (int t = 0; t < 6; ++t) pers[t] = 0;

                /* determine which old components still have a cell in this row */
                for (int j = 0; j < W; ++j) {
                    int id = prev_lab[j];
                    if (id >= 0 && (m & (1 << j))) pers[id] = 1;
                }

                /* UL component must survive */
                if (!pers[0]) continue;

                /* any other component that disappears => invalid */
                bool bad = false;
                for (int id = 1; id <= max_old; ++id)
                    if (seen[id] && !pers[id]) { bad = true; break; }
                if (bad) continue;

                /* build union-find for this row */
                unsigned p[12];
                for (int ii = 0; ii < 12; ++ii) p[ii] = ii;
                unsigned col2uf[6];
                unsigned uf_next = 6;                 /* 0..5 are old component ids */
                for (int j = 0; j < W; ++j) {
                    if (m & (1 << j)) {
                        int pid = prev_lab[j];
                        if (pid >= 0) col2uf[j] = (unsigned)pid;
                        else          col2uf[j] = uf_next++;
                    } else
                        col2uf[j] = 0xFFFFFFFFu;
                }
                /* horizontal unions */
                for (int j = 1; j < W; ++j)
                    if ((m & (1 << j)) && (m & (1 << (j - 1))))
                        union_sets(col2uf[j - 1], col2uf[j], p);

                /* canonical labeling */
                int root_to_lbl[12];
                for (int ii = 0; ii < 12; ++ii) root_to_lbl[ii] = -1;
                root_to_lbl[find_root(0, p)] = 0;
                int next_lbl = 1;
                int lab[6];
                for (int j = 0; j < W; ++j) {
                    if (m & (1 << j)) {
                        unsigned r = find_root(col2uf[j], p);
                        if (root_to_lbl[r] == -1) root_to_lbl[r] = next_lbl++;
                        lab[j] = root_to_lbl[r];
                    } else
                        lab[j] = -1;
                }
                uint32_t new_key; encode_state(lab, &new_key);
                hash_insert(new_key, ways);
            }
        }

        /* transfer hash entries to curr array for next iteration */
        curr_len = 0;
        for (int i = 0; i < used_cnt; ++i) {
            uint32_t idx = used_idx[i];
            curr[curr_len].key = hash_tbl[idx].key;
            curr[curr_len++].val = hash_tbl[idx].val;
        }
    }

    /* ---------- final answer: LR cell must be 1 and part of UL component,
     * and all 1’s must belong to the UL component (no other components)  */
    seq_t result = 0;
    for (int i = 0; i < curr_len; ++i) {
        int lab[6];
        decode_state(curr[i].key, lab);
        if (lab[5] == 0) {
            bool ok = true;
            for (int j = 0; j < 6; ++j)
                if (lab[j] > 0) { ok = false; break; }
            if (ok) result += curr[i].val;
        }
    }
    return result;
}
