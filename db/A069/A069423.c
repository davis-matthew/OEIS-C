/*
 * A069423  Number of n X 3 binary arrays with a path of adjacent 1's and no
 *          path of adjacent 0's from top row to bottom row.
 * https://oeis.org/A069423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computes A069423 by dynamic programming over rows * (enumerates
 *            all 3‑column binary arrays and tracks * connectivity of
 *            1‑components and 0‑components)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   state_meets_criteria
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/state_meets_criteria.h"

/* Helper to perform union–find operations on 0..5 indices */
static inline int uf_find(int *parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

/* Encode a frontier state: 3 mask bits (mask), 3 component IDs (0..2),
 * 3 top‑connection flags.  The digits are packed into a 12‑bit integer. */
static inline int encode_state(uint8_t mask,
                               uint8_t comp0, uint8_t comp1, uint8_t comp2,
                               uint8_t top0, uint8_t top1, uint8_t top2) {
    int code = mask & 0x7;
    code |= (comp0 & 0x3) << 3;
    code |= (comp1 & 0x3) << 5;
    code |= (comp2 & 0x3) << 7;
    code |= (top0 & 1) << 9;
    code |= (top1 & 1) << 10;
    code |= (top2 & 1) << 11;
    return code;
}

/* Build the frontier state for the first row given its mask. */
static inline int init_state_from_mask(int mask) {
    int parent[3];
    int flag[3];
    for (int i = 0; i < 3; i++) {
        parent[i] = i;
        flag[i] = 1;  /* top row is automatically top‑connected */
    }
    /* Horizontal merges */
    for (int c = 0; c < 2; c++) {
        int v1 = (mask >> c) & 1;
        int v2 = (mask >> (c + 1)) & 1;
        if (v1 == v2) {
            int r1 = uf_find(parent, c);
            int r2 = uf_find(parent, c + 1);
            if (r1 != r2) {
                parent[r1] = r2;
                flag[r2] |= flag[r1];
            }
        }
    }
    /* Compress and assign compact IDs */
    int rep[3];
    for (int c = 0; c < 3; c++) rep[c] = uf_find(parent, c);
    int map[6];
    for (int i = 0; i < 6; i++) map[i] = -1;
    int nextId = 0;
    for (int c = 0; c < 3; c++) {
        int r = rep[c];
        if (map[r] == -1) map[r] = nextId++;
    }
    int comp[3];
    int top[3] = {0, 0, 0};
    for (int c = 0; c < 3; c++) {
        int r = rep[c];
        int id = map[r];
        comp[c] = id;
        top[id] = 1;            /* all components are top‑connected */
    }
    return encode_state(mask, comp[0], comp[1], comp[2], top[0], top[1], top[2]);
}

/* Transition from previous frontier state to next frontier state
 * given the mask of the current row. */
static inline int transition_state(int prevCode, int mask) {
    /* Decode previous state */
    int pmask = prevCode & 0x7;
    int pcomp[3] = { (prevCode >> 3) & 3,
                     (prevCode >> 5) & 3,
                     (prevCode >> 7) & 3 };
    int ptop[3]  = { (prevCode >> 9) & 1,
                     (prevCode >>10) & 1,
                     (prevCode >>11) & 1 };

    /* Provisional IDs and DSU structures */
    int parent[6];
    int flag  [6];
    /* Existing component IDs 0..2 */
    for (int i = 0; i < 3; i++) {
        parent[i] = i;
        flag[i]   = ptop[i];
    }
    int baseNew = 3, nextNew = 0;
    int tmpid[3];
    for (int c = 0; c < 3; c++) {
        int curVal = (mask >> c) & 1;
        int prevVal = (pmask >> c) & 1;
        if (curVal == prevVal) {
            tmpid[c] = pcomp[c];
        } else {
            int nid = baseNew + nextNew++;
            tmpid[c] = nid;
            parent[nid] = nid;
            flag[nid]   = 0;             /* new component not top‑connected yet */
        }
    }

    /* Horizontal unions */
    for (int c = 0; c < 2; c++) {
        int curVal = (mask >> c) & 1;
        int nxtVal = (mask >> (c + 1)) & 1;
        if (curVal == nxtVal) {
            int a = tmpid[c];
            int b = tmpid[c + 1];
            int ra = uf_find(parent, a);
            int rb = uf_find(parent, b);
            if (ra != rb) {
                parent[ra] = rb;
                flag[rb] |= flag[ra];
            }
        }
    }

    /* Collect representatives and reindex to 0..k-1 */
    int rep[3];
    for (int c = 0; c < 3; c++) rep[c] = uf_find(parent, tmpid[c]);

    int map[6];
    for (int i = 0; i < 6; i++) map[i] = -1;
    int nextId = 0;
    for (int c = 0; c < 3; c++) {
        int r = rep[c];
        if (map[r] == -1) map[r] = nextId++;
    }

    int comp[3];
    int top[3] = {0, 0, 0};
    for (int c = 0; c < 3; c++) {
        int r = rep[c];
        int id = map[r];
        comp[c] = id;
        top[id] = flag[r];
    }

    return encode_state(mask, comp[0], comp[1], comp[2], top[0], top[1], top[2]);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Allocate cache if this is the first call. */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_size = 0;
    }

    /* Return cached result if available. */
    if (n <= cache_size) return cache[n];

    /* Expand cache to accommodate up to n. */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = cache_size + 1; i <= n; i++) cache[i] = (seq_t)0;
    cache_size = n;

    /* Static DP state memory. */
    static seq_t *cur = NULL, *nxt = NULL;
    static idx_t processed_rows = 0;        /* number of rows already processed */

    /* Initialize DP if first call. */
    if (processed_rows == 0) {
        cur = (seq_t *)malloc(4096 * sizeof(seq_t));
        nxt = (seq_t *)malloc(4096 * sizeof(seq_t));
        memset(cur, 0, 4096 * sizeof(seq_t));

        /* First row: all 8 masks. */
        for (int mask = 0; mask < 8; mask++) {
            int code = init_state_from_mask(mask);
            cur[code] = (seq_t)1;
        }
        processed_rows = 1;

        /* Compute result for n==1. */
        seq_t total = 0;
        for (int code = 0; code < 4096; code++)
            if (cur[code] && state_meets_criteria(code))
                total += cur[code];
        cache[1] = total;
    }

    /* Extend DP to reach required n. */
    while (processed_rows < n) {
        /* Transition to next row. */
        memset(nxt, 0, 4096 * sizeof(seq_t));
        for (int code = 0; code < 4096; code++) {
            seq_t cnt = cur[code];
            if (!cnt) continue;
            for (int mask = 0; mask < 8; mask++) {
                int next_code = transition_state(code, mask);
                nxt[next_code] += cnt;
            }
        }
        /* Swap current and next. */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
        processed_rows++;

        /* If we just finished the desired number of rows, compute result. */
        if (processed_rows == n) {
            seq_t total = 0;
            for (int code = 0; code < 4096; code++)
                if (cur[code] && state_meets_criteria(code))
                    total += cur[code];
            cache[n] = total;
        }
    }

    return cache[n];
}
