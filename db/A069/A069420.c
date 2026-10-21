/*
 * A069420  Number of 6 X n binary arrays with a path of adjacent 1's and no
 *          path of adjacent 0's from top row to bottom row.
 * https://oeis.org/A069420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of 6×n binary arrays with a path of adjacent 1's from top
 *            to bottom and no such path for 0's. Implemented by a
 *            transfer‑matrix DP over columns, tracking component connectivity
 *            for both colours
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define ROWS 6

typedef struct {
    uint8_t col_bits;
    uint8_t comp1[ROWS];
    uint8_t comp0[ROWS];
    uint8_t topMask1;
    uint8_t bottomMask1;
    uint8_t topMask0;
    uint8_t bottomMask0;
    uint8_t per1;
    uint8_t per0;
} State;

/* initialise arrays to 0xFF */
static inline void init_comp(uint8_t arr[ROWS]) {
    for (int i = 0; i < ROWS; ++i) arr[i] = 0xFF;
}

/* union‑find helpers */
static inline int uf_find(int v, int parent[]) {
    while (parent[v] != v) {
        parent[v] = parent[parent[v]];
        v = parent[v];
    }
    return v;
}
static inline void uf_union(int a, int b, int parent[]) {
    a = uf_find(a, parent);
    b = uf_find(b, parent);
    if (a != b) parent[a] = b;
}

/* process one Colour (1 or 0) */
static void process_colour(const uint8_t prev_comp[ROWS],
                           uint8_t topPrevMask, uint8_t bottomPrevMask,
                           uint8_t prevMask,        /* previous column bits */
                           uint8_t newMask,         /* new column bits */
                           uint8_t out_comp[ROWS],
                           uint8_t *out_topMask,
                           uint8_t *out_bottomMask,
                           uint8_t *percolated)
{
    /* --- new column local components --- */
    uint8_t new_comp[ROWS];
    uint8_t newTopMask = 0, newBottomMask = 0;
    int cur = 0;
    for (int i = 0; i < ROWS; ++i) {
        bool has = (newMask >> i) & 1;
        if (!has) {
            new_comp[i] = 0xFF;
            continue;
        }
        if (i > 0 && ((newMask >> (i - 1)) & 1)) {
            new_comp[i] = new_comp[i - 1];
        } else {
            new_comp[i] = cur++;
        }
        if (i == 0) newTopMask |= 1u << new_comp[i];
        if (i == ROWS - 1) newBottomMask |= 1u << new_comp[i];
    }
    int kNew = cur;

    /* previous components count max ID */
    int kPrev = 0;
    for (int i = 0; i < ROWS; ++i) {
        if (prev_comp[i] != 0xFF)
            kPrev = (prev_comp[i] + 1 > kPrev) ? prev_comp[i] + 1 : kPrev;
    }

    if (kPrev == 0 && kNew == 0) {
        *percolated = 0;
        *out_topMask = *out_bottomMask = 0;
        init_comp(out_comp);
        return;
    }

    int tot = kPrev + kNew;
    int parent[12];
    for (int i = 0; i < tot; ++i) parent[i] = i;

    /* horizontal adjacency */
    for (int i = 0; i < ROWS; ++i) {
        bool havePrev = (prevMask >> i) & 1;
        bool haveNew  = (newMask >> i) & 1;
        if (havePrev && haveNew) {
            int p = prev_comp[i];
            int n = new_comp[i];
            uf_union(p, kPrev + n, parent);
        }
    }

    /* root information */
    int rootHasNew[12] = {0};
    int rootTop[12]    = {0};
    int rootBottom[12] = {0};

    /* previous components */
    for (int i = 0; i < ROWS; ++i) {
        int id = prev_comp[i];
        if (id == 0xFF) continue;
        int r = uf_find(id, parent);
        if ((topPrevMask >> id) & 1) rootTop[r] = 1;
        if ((bottomPrevMask >> id) & 1) rootBottom[r] = 1;
    }

    /* new components */
    for (int i = 0; i < ROWS; ++i) {
        int nid = new_comp[i];
        if (nid == 0xFF) continue;
        int r = uf_find(kPrev + nid, parent);
        rootHasNew[r] = 1;
        if ((newTopMask >> nid) & 1) rootTop[r] = 1;
        if ((newBottomMask >> nid) & 1) rootBottom[r] = 1;
    }

    /* mapping from root to new component numbers */
    int mapping[12];
    for (int i = 0; i < tot; ++i) mapping[i] = -1;
    int newid = 0;
    *out_topMask = *out_bottomMask = 0;
    for (int r = 0; r < tot; ++r) {
        if (!rootHasNew[r]) continue;
        mapping[r] = newid;
        if (rootTop[r])   *out_topMask   |= 1u << newid;
        if (rootBottom[r])*out_bottomMask|= 1u << newid;
        ++newid;
    }

    /* percolation check */
    *percolated = 0;
    for (int r = 0; r < tot; ++r) {
        if (rootHasNew[r] && rootTop[r] && rootBottom[r]) {
            *percolated = 1;
            break;
        }
    }
    if (*percolated) {
        init_comp(out_comp);
        *out_topMask = *out_bottomMask = 0;
        return;
    }

    /* assign component numbers for new column */
    for (int i = 0; i < ROWS; ++i) {
        int nid = new_comp[i];
        if (nid == 0xFF) {
            out_comp[i] = 0xFF;
        } else {
            int r = uf_find(kPrev + nid, parent);
            out_comp[i] = mapping[r];
        }
    }
}

static State init_state(uint8_t mask) {
    State s;
    s.col_bits = mask;
    uint8_t dummy[ROWS];
    init_comp(dummy);
    uint8_t topPrevMask = 0, bottomPrevMask = 0, prevMask = 0;

    uint8_t comp1[ROWS], topM1, bottomM1, per1;
    process_colour(dummy, topPrevMask, bottomPrevMask, prevMask, mask,
                   comp1, &topM1, &bottomM1, &per1);

    uint8_t comp0[ROWS], topM0, bottomM0, per0;
    process_colour(dummy, topPrevMask, bottomPrevMask, prevMask,
                   (~mask) & 0x3F,
                   comp0, &topM0, &bottomM0, &per0);

    memcpy(s.comp1, comp1, ROWS);
    memcpy(s.comp0, comp0, ROWS);
    s.topMask1 = topM1; s.bottomMask1 = bottomM1;
    s.topMask0 = topM0; s.bottomMask0 = bottomM0;
    s.per1 = per1; s.per0 = per0;
    return s;
}

static State transition_state(const State *prev, uint8_t mask) {
    State s;
    s.col_bits = mask;

    uint8_t prevMask1 = prev->col_bits;
    uint8_t prevMask0 = (~prev->col_bits) & 0x3F;
    uint8_t newMask1 = mask;
    uint8_t newMask0 = (~mask) & 0x3F;

    uint8_t comp1[ROWS], topM1, bottomM1, per1;
    if (prev->per1) {
        per1 = 1;
        init_comp(comp1);
        topM1 = bottomM1 = 0;
    } else {
        process_colour(prev->comp1, prev->topMask1, prev->bottomMask1,
                       prevMask1, newMask1,
                       comp1, &topM1, &bottomM1, &per1);
    }

    uint8_t comp0[ROWS], topM0, bottomM0, per0;
    if (prev->per0) {
        per0 = 1;
        init_comp(comp0);
        topM0 = bottomM0 = 0;
    } else {
        process_colour(prev->comp0, prev->topMask0, prev->bottomMask0,
                       prevMask0, newMask0,
                       comp0, &topM0, &bottomM0, &per0);
    }

    memcpy(s.comp1, comp1, ROWS);
    memcpy(s.comp0, comp0, ROWS);
    s.topMask1 = topM1; s.bottomMask1 = bottomM1;
    s.topMask0 = topM0; s.bottomMask0 = bottomM0;
    s.per1 = per1; s.per0 = per0;
    return s;
}

typedef struct {
    State s;
    seq_t val;
    unsigned char used;
} TableEntry;

typedef struct {
    TableEntry *data;
    size_t cap;
    size_t used;
} Table;

static size_t state_hash(const State *st) {
    size_t h = 1469598103934665603ULL;
    h ^= st->col_bits;
    h *= 1099511628211ULL;
    for (int i = 0; i < ROWS; ++i) {
        h ^= st->comp1[i];
        h *= 1099511628211ULL;
        h ^= st->comp0[i];
        h *= 1099511628211ULL;
    }
    h ^= st->topMask1; h *= 1099511628211ULL;
    h ^= st->bottomMask1; h *= 1099511628211ULL;
    h ^= st->topMask0; h *= 1099511628211ULL;
    h ^= st->bottomMask0; h *= 1099511628211ULL;
    h ^= st->per1; h *= 1099511628211ULL;
    h ^= st->per0; h *= 1099511628211ULL;
    return h;
}

static bool same_state(const State *a, const State *b) {
    if (a->col_bits != b->col_bits) return false;
    if (memcmp(a->comp1, b->comp1, ROWS) != 0) return false;
    if (memcmp(a->comp0, b->comp0, ROWS) != 0) return false;
    if (a->topMask1 != b->topMask1) return false;
    if (a->bottomMask1 != b->bottomMask1) return false;
    if (a->topMask0 != b->topMask0) return false;
    if (a->bottomMask0 != b->bottomMask0) return false;
    if (a->per1 != b->per1) return false;
    return a->per0 == b->per0;
}

static void table_init(Table *t, size_t cap) {
    t->cap = cap;
    t->used = 0;
    t->data = (TableEntry*)calloc(cap, sizeof(TableEntry));
}

static void table_clear(Table *t) {
    memset(t->data, 0, t->cap * sizeof(TableEntry));
    t->used = 0;
}

static void table_resize(Table *t, size_t newcap) {
    TableEntry *old = t->data;
    size_t oldcap = t->cap;
    t->data = (TableEntry*)calloc(newcap, sizeof(TableEntry));
    t->cap = newcap;
    t->used = 0;
    for (size_t i = 0; i < oldcap; ++i) {
        if (!old[i].used) continue;
        const State *k = &old[i].s;
        seq_t v = old[i].val;
        size_t h = state_hash(k) & (newcap - 1);
        while (t->data[h].used) h = (h + 1) & (newcap - 1);
        t->data[h].s = *k;
        t->data[h].val = v;
        t->data[h].used = 1;
        ++t->used;
    }
    free(old);
}

static void table_put(Table *t, const State *key, seq_t val) {
    if (t->used > t->cap / 2) {
        table_resize(t, t->cap * 2);
    }
    size_t h = state_hash(key) & (t->cap - 1);
    while (1) {
        if (!t->data[h].used) {
            t->data[h].s = *key;
            t->data[h].val = val;
            t->data[h].used = 1;
            ++t->used;
            return;
        }
        if (same_state(key, &t->data[h].s)) {
            t->data[h].val += val;
            return;
        }
        h = (h + 1) & (t->cap - 1);
    }
}

static void table_iter(const Table *t, void (*func)(const State*, seq_t)) {
    for (size_t i = 0; i < t->cap; ++i) {
        if (t->data[i].used) func(&t->data[i].s, t->data[i].val);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 */

    size_t initCap = 1 << 18;
    Table cur, nxt;
    table_init(&cur, initCap);
    table_init(&nxt, initCap);

    for (uint8_t mask = 0; mask < 64; ++mask) {
        State st = init_state(mask);
        table_put(&cur, &st, 1);
    }

    for (idx_t col = 2; col <= n; ++col) {
        table_clear(&nxt);
        for (size_t i = 0; i < cur.cap; ++i) {
            if (!cur.data[i].used) continue;
            const State *prev = &cur.data[i].s;
            seq_t cnt = cur.data[i].val;
            for (uint8_t mask = 0; mask < 64; ++mask) {
                State next = transition_state(prev, mask);
                table_put(&nxt, &next, cnt);
            }
        }
        Table tmp = cur; cur = nxt; nxt = tmp;
    }

    seq_t ans = 0;
    for (size_t i = 0; i < cur.cap; ++i) {
        if (!cur.data[i].used) continue;
        const State *s = &cur.data[i].s;
        if (s->per1 && !s->per0) ans += cur.data[i].val;
    }
    return ans;
}
