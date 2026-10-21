/*
 * A069307  Number of 3 X n binary arrays with a path of adjacent 1's from
 *          upper left corner to anywhere in right hand column.
 * https://oeis.org/A069307
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A069307.
   It uses a transfer‑matrix (counting frontier states) to enumerate
   all 3×n binary arrays that contain a path of adjacent 1’s from the
   cell (0,0) to some cell in the rightmost column.
   The result is returned in the type seq_t; the index is idx_t. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* state of a frontier column */
struct State {
    int8_t comp[3];      /* component id of each row (-1 if absent)  */
    uint8_t root[3];     /* root flag for each component (0/1)      */
};

/* equivalence test for states */
static bool state_eq(const struct State *a, const struct State *b)
{
    return a->comp[0] == b->comp[0] &&
           a->comp[1] == b->comp[1] &&
           a->comp[2] == b->comp[2] &&
           a->root[0] == b->root[0] &&
           a->root[1] == b->root[1] &&
           a->root[2] == b->root[2];
}

/* union–find helpers (at most 6 nodes) ---------------------------------- */
static int uf_find(int id, int *parent)
{
    int root = id;
    while (parent[root] != root)
        root = parent[root];
    while (parent[id] != root) {
        int p = parent[id];
        parent[id] = root;
        id = p;
    }
    return root;
}

static void uf_union(int a, int b, int *parent, uint8_t *rootFlag)
{
    int ra = uf_find(a, parent);
    int rb = uf_find(b, parent);
    if (ra == rb) return;
    parent[ra] = rb;
    rootFlag[rb] = rootFlag[rb] | rootFlag[ra];
}

/* initial state for a column mask (bit 0 always set) --------------------- */
static struct State init_state_from_mask(unsigned mask)
{
    struct State st = { { -1, -1, -1 }, { 0, 0, 0 } };
    int parent[3];
    uint8_t rootFlag[3];

    for (int r = 0; r < 3; ++r) {
        if (mask & (1u << r)) {
            parent[r] = r;
            rootFlag[r] = (r == 0) ? 1 : 0;   /* row 0 is the root          */
        } else {
            parent[r] = -1;
            rootFlag[r] = 0;
        }
    }

    /* vertical adjacency inside the column */
    if ((mask & 0x1u) && (mask & 0x2u)) uf_union(0, 1, parent, rootFlag);
    if ((mask & 0x2u) && (mask & 0x4u)) uf_union(1, 2, parent, rootFlag);

    /* canonical component ids */
    int mapFromRoot[3] = { -1, -1, -1 };
    int cid = 0;
    for (int r = 0; r < 3; ++r) {
        if (mask & (1u << r)) {
            int rep = uf_find(r, parent);
            if (mapFromRoot[rep] == -1) mapFromRoot[rep] = cid++;
            st.comp[r] = mapFromRoot[rep];
            st.root[st.comp[r]] = rootFlag[rep];
        }
    }
    return st;
}

/* transition from previous state to new column mask --------------------- */
static struct State transition(const struct State *prev, unsigned oldMask,
                               unsigned newMask)
{
    struct State res = { { -1, -1, -1 }, { 0, 0, 0 } };

    int prevCompToUF[3] = { -1, -1, -1 };
    int nextUF = 0;
    int rowToUF[3];
    uint8_t ufRootFlag[6];            /* at most 3 old + 3 new nodes */

    for (int r = 0; r < 3; ++r) {
        if (!(newMask & (1u << r))) {
            rowToUF[r] = -1;
            continue;
        }
        if (oldMask & (1u << r)) {
            int pid = prev->comp[r];
            if (prevCompToUF[pid] == -1) {
                prevCompToUF[pid] = nextUF;
                ufRootFlag[nextUF] = prev->root[pid];
                ++nextUF;
            }
            rowToUF[r] = prevCompToUF[pid];
        } else {
            rowToUF[r] = nextUF;
            ufRootFlag[nextUF] = 0;   /* new node initially not connected to root */
            ++nextUF;
        }
    }

    int totalUF = nextUF;
    int parent[6];
    for (int i = 0; i < totalUF; ++i) parent[i] = i;

    /* vertical adjacency within the new column */
    for (int r = 0; r < 2; ++r) {
        if ((newMask & (1u << r)) && (newMask & (1u << (r + 1))))
            uf_union(rowToUF[r], rowToUF[r + 1], parent, ufRootFlag);
    }

    int rootToCID[6];
    for (int i = 0; i < totalUF; ++i) rootToCID[i] = -1;
    int cid = 0;

    for (int r = 0; r < 3; ++r) {
        if (!(newMask & (1u << r))) {
            res.comp[r] = -1;
            continue;
        }
        int rep = uf_find(rowToUF[r], parent);
        if (rootToCID[rep] == -1) {
            rootToCID[rep] = cid;
            res.root[cid] = ufRootFlag[rep];
            ++cid;
        }
        res.comp[r] = rootToCID[rep];
    }
    return res;
}

/* maximum number of frontier states (enough for 3×n) */
#define MAX_STATES 512

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* offset not defined */

    /* frontier structures for current column */
    struct State curStates[MAX_STATES];
    seq_t curCounts[MAX_STATES];
    size_t curSize = 0;

    /* initialise first column (root at row 0 must be present) */
    unsigned initMasks[4] = { 0x1u, 0x3u, 0x5u, 0x7u };
    for (int i = 0; i < 4; ++i) {
        struct State st = init_state_from_mask(initMasks[i]);
        curStates[curSize] = st;
        curCounts[curSize] = (seq_t)1;
        ++curSize;
    }

    /* process columns 2 … n */
    for (idx_t col = 2; col <= n; ++col) {
        struct State nextStates[MAX_STATES];
        seq_t nextCounts[MAX_STATES];
        size_t nextSize = 0;

        for (size_t idx = 0; idx < curSize; ++idx) {
            const struct State *prev = &curStates[idx];
            seq_t prevCount = curCounts[idx];

            unsigned oldMask = 0;
            for (int r = 0; r < 3; ++r)
                if (prev->comp[r] != -1)
                    oldMask |= (1u << r);

            for (unsigned newMask = 0; newMask < 8; ++newMask) {
                struct State newSt = transition(prev, oldMask, newMask);
                /* search for an existing equivalent state */
                size_t pos;
                for (pos = 0; pos < nextSize; ++pos)
                    if (state_eq(&nextStates[pos], &newSt))
                        break;
                if (pos == nextSize) {          /* new state */
                    nextStates[nextSize] = newSt;
                    nextCounts[nextSize] = prevCount;
                    ++nextSize;
                } else {                       /* accumulate */
                    nextCounts[pos] = nextCounts[pos] + prevCount;
                }
            }
        }

        /* prepare for next column */
        curSize = nextSize;
        for (size_t i = 0; i < curSize; ++i) {
            curStates[i] = nextStates[i];
            curCounts[i] = nextCounts[i];
        }
    }

    /* sum counts of final states whose component containing the root
       touches the right column (i.e., root flag present) */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < curSize; ++i) {
        const struct State *st = &curStates[i];
        bool rootPresent = false;
        for (int r = 0; r < 3 && !rootPresent; ++r) {
            if (st->comp[r] != -1 && st->root[st->comp[r]] == 1)
                rootPresent = true;
        }
        if (rootPresent)
            ans = ans + curCounts[i];
    }

    return ans;
}
