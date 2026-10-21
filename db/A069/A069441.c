/*
 * A069441  Half the number of n X 3 binary arrays with no path of adjacent
 *          1's or adjacent 0's from top row to bottom row.
 * https://oeis.org/A069441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   uf_find
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 2^(3n-1) - A069378(n) + A069403(n).
 * The DP counts all n×3 binary arrays with *no* monochromatic (all 0's or all 1's)
 * path from the top row to the bottom row.  Since the OEIS sequence gives
 * half of that count, the result is divided by two at the end. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/uf_find.h"

/* maximum number of admissible states (sufficiently large) */
#define MAX_STATES 2000

/* state definition ----------------------------------------------------- */
typedef struct {
    unsigned char comp[3];    /* component id for each column (0..2) */
    unsigned char color[3];   /* bit value for each component (0 or 1) */
    unsigned char top[3];     /* whether component touches the top row */
    unsigned char comps;      /* number of distinct components (1..3) */
} State;

/* global tables -------------------------------------------------------- */
static State states[MAX_STATES];
static int   state_count;
static int   trans[MAX_STATES][8];     /* trans[i][pat] = next state index */
static bool  good_state[MAX_STATES];  /* true if no component touches the top */

/* one‑time initialisation flag --------------------------------------- */
static bool init_done = false;

/* --------------------------------------------------------------------- */
static void build_all_states(void) {
    state_count = 0;
    /* all raw assignments of columns to raw IDs 0..2 (not necessarily distinct) */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c) {
                unsigned char raw[3] = { (unsigned char)a, (unsigned char)b, (unsigned char)c };
                /* canonicalise raw ids: first appearance gets 0,1,2...    */
                unsigned char canon[3], map[3] = {255,255,255}, next = 0;
                for (int i = 0; i < 3; ++i) {
                    int id = raw[i];
                    if (map[id] == 255) map[id] = next++;
                    canon[i] = map[id];
                }
                /* count distinct components */
                unsigned char comps = 0;
                bool seen[3] = {0,0,0};
                for (int i = 0; i < 3; ++i) {
                    int id = canon[i];
                    if (!seen[id]) { seen[id] = 1; comps++; }
                }
                /* iterate over all possible color and top masks for the components */
                for (int cMask = 0; cMask < (1 << comps); ++cMask)
                    for (int tMask = 0; tMask < (1 << comps); ++tMask) {
                        State s;
                        s.comps = comps;
                        for (int i = 0; i < 3; ++i) s.comp[i] = canon[i];
                        for (int cid = 0; cid < comps; ++cid) {
                            s.color[cid] = (cMask >> cid) & 1;
                            s.top[cid]   = (tMask >> cid) & 1;
                        }
                        /* insert if not already present */
                        bool dup = false;
                        for (int k = 0; k < state_count; ++k) {
                            if (states[k].comps != s.comps) continue;
                            bool eq = true;
                            for (int i = 0; i < 3; ++i)
                                if (states[k].comp[i] != s.comp[i]) { eq = false; break; }
                            if (!eq) continue;
                            for (int cid = 0; cid < comps; ++cid)
                                if (states[k].color[cid] != s.color[cid] ||
                                    states[k].top[cid]   != s.top[cid]) { eq = false; break; }
                            if (eq) { dup = true; break; }
                        }
                        if (!dup) {
                            states[state_count++] = s;
                        }
                    }
            }
    /* mark good states (no component touches the top) */
    for (int i = 0; i < state_count; ++i) {
        bool ok = true;
        for (int cid = 0; cid < states[i].comps; ++cid)
            if (states[i].top[cid]) { ok = false; break; }
        good_state[i] = ok;
    }
}

/* --------------------------------------------------------------------- */
static int find_state_index(const State *s) {
    for (int i = 0; i < state_count; ++i) {
        if (states[i].comps != s->comps) continue;
        bool eq = true;
        for (int j = 0; j < 3; ++j)
            if (states[i].comp[j] != s->comp[j]) { eq = false; break; }
        if (!eq) continue;
        for (int cid = 0; cid < s->comps; ++cid)
            if (states[i].color[cid] != s->color[cid] ||
                states[i].top[cid]   != s->top[cid]) { eq = false; break; }
        if (eq) return i;
    }
    return -1;  /* should never happen */
}

/* --------------------------------------------------------------------- */
static void compute_next_state(const State *cur, int pat, State *next) {
    unsigned char col[3];
    for (int i = 0; i < 3; ++i) col[i] = (pat >> i) & 1;
    int parent[6];
    unsigned char uColor[6];
    unsigned char uTop[6];
    int idMap[3];
    int nextFree = cur->comps;

    /* initialise existing components */
    for (int cid = 0; cid < cur->comps; ++cid) {
        parent[cid] = cid;
        uColor[cid] = cur->color[cid];
        uTop[cid]   = cur->top[cid];
    }

    /* assign columns to components, create new ones if colors differ */
    for (int i = 0; i < 3; ++i) {
        unsigned char oldCid = cur->comp[i];
        unsigned char oldCol = cur->color[oldCid];
        if (col[i] == oldCol)
            idMap[i] = oldCid;   /* vertical match – keep the component */
        else {
            idMap[i] = nextFree; /* new component born in this row */
            parent[nextFree] = nextFree;
            uColor[nextFree] = col[i];
            uTop[nextFree]   = 0;
            ++nextFree;
        }
    }

    /* horizontal merges within the same row */
    for (int i = 0; i < 2; ++i) {
        if (col[i] == col[i+1]) {
            int a = uf_find(parent, idMap[i]);
            int b = uf_find(parent, idMap[i+1]);
            if (a != b) {
                parent[b] = a;
                uTop[a] |= uTop[b];
            }
        }
    }

    /* canonicalise component ids */
    int repToNew[6];
    for (int i = 0; i < 6; ++i) repToNew[i] = -1;
    int newCompCnt = 0;
    for (int i = 0; i < 3; ++i) {
        int rep = uf_find(parent, idMap[i]);
        if (repToNew[rep] < 0) repToNew[rep] = newCompCnt++;
        next->comp[i] = repToNew[rep];
    }
    next->comps = newCompCnt;

    /* copy color and top flags for each new component */
    for (int cid = 0; cid < newCompCnt; ++cid) {
        int rep = -1;
        for (int j = 0; j < 6; ++j)
            if (repToNew[j] == cid) { rep = j; break; }
        next->color[cid] = uColor[rep];
        next->top[cid]   = uTop[rep];
    }
}

/* --------------------------------------------------------------------- */
static void build_start_state(int pat, State *s) {
    unsigned char col[3];
    for (int i = 0; i < 3; ++i) col[i] = (pat >> i) & 1;
    /* each column starts as a separate component and touches the top */
    for (int i = 0; i < 3; ++i) {
        s->comp[i] = (unsigned char)i;
        s->color[i] = col[i];
        s->top[i] = 1;
    }
    s->comps = 3;
    /* horizontal merges within the first row */
    int parent[3];
    for (int i = 0; i < 3; ++i) parent[i] = i;
    for (int i = 0; i < 2; ++i) {
        if (col[i] == col[i+1]) {
            int a = uf_find(parent, i);
            int b = uf_find(parent, i+1);
            if (a != b) parent[b] = a;
        }
    }
    int repToNew[3];
    for (int i = 0; i < 3; ++i) repToNew[i] = -1;
    int newCompCnt = 0;
    for (int i = 0; i < 3; ++i) {
        int rep = uf_find(parent, i);
        if (repToNew[rep] < 0) repToNew[rep] = newCompCnt++;
        s->comp[i] = repToNew[rep];
    }
    s->comps = newCompCnt;
    for (int cid = 0; cid < newCompCnt; ++cid) {
        int rep = -1;
        for (int i = 0; i < 3; ++i)
            if (repToNew[i] == cid) { rep = i; break; }
        s->color[cid] = col[rep];
        s->top[cid]   = 1;
    }
}

/* --------------------------------------------------------------------- */
static void build_transitions(void) {
    for (int i = 0; i < state_count; ++i)
        for (int pat = 0; pat < 8; ++pat) {
            State nxt;
            compute_next_state(&states[i], pat, &nxt);
            trans[i][pat] = find_state_index(&nxt);
        }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (!init_done) {
        build_all_states();
        build_transitions();
        init_done = true;
    }

    static seq_t dp[MAX_STATES];
    static seq_t dp_next[MAX_STATES];
    for (int i = 0; i < state_count; ++i) dp[i] = 0;

    /* initial row: each of the 8 patterns is a valid start */
    for (int pat = 0; pat < 8; ++pat) {
        State st;
        build_start_state(pat, &st);
        int idx = find_state_index(&st);
        dp[idx] = dp[idx] + 1;
    }

    /* process the remaining rows, if any */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < state_count; ++i) dp_next[i] = 0;
        for (int cur = 0; cur < state_count; ++cur) {
            seq_t val = dp[cur];
            if (!val) continue;
            for (int pat = 0; pat < 8; ++pat) {
                int nxt = trans[cur][pat];
                dp_next[nxt] = dp_next[nxt] + val;
            }
        }
        for (int i = 0; i < state_count; ++i) dp[i] = dp_next[i];
    }

    /* sum counts of all “good” states (no component touches the top) */
    seq_t ans = 0;
    for (int i = 0; i < state_count; ++i)
        if (good_state[i]) ans = ans + dp[i];

    /* the sequence is defined as half of this count */
    return ans / 2;
}
