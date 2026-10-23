/*
 * A169018  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed via Aho‑Corasick automaton to avoid forbidden
 *            substrings (adjacent equal letters and alternating patterns of
 *            length 24 for any pair of generators)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Alphabet size (5 generators). */
#define ALPH 5
/* Maximum number of automaton nodes needed: sum of pattern lengths + 1. */
/* 25 patterns (5 of length 2 + 10 unordered pairs * 2 patterns of length 24)
   each up to length 24 → 25*24 = 600, plus root. 800 is safe. */
#define MAX_NODES 800

typedef struct {
    int next[ALPH];
    int fail;
    unsigned char out;   /* 1 if this state corresponds to a forbidden word suffix. */
} Node;

static Node nodes[MAX_NODES];
static int node_count;
static int go_table[MAX_NODES][ALPH];
static unsigned char bad_state[MAX_NODES];
static int initialized = 0;

/* Insert a pattern of length len consisting of alphabet symbols 0..4. */
static void insert_pattern(const int *pat, int len)
{
    int v = 0;
    for (int i = 0; i < len; ++i) {
        int c = pat[i];
        if (nodes[v].next[c] == -1) {
            int u = node_count++;
            for (int j = 0; j < ALPH; ++j) nodes[u].next[j] = -1;
            nodes[u].fail = 0;
            nodes[u].out = 0;
            nodes[v].next[c] = u;
        }
        v = nodes[v].next[c];
    }
    nodes[v].out = 1;
}

/* Build the automaton and precompute transition table. */
static void build_automaton(void)
{
    /* Initialize root node. */
    node_count = 1;
    for (int j = 0; j < ALPH; ++j) nodes[0].next[j] = -1;
    nodes[0].fail = 0;
    nodes[0].out = 0;

    /* Forbidden patterns of length 2: "aa" for each letter. */
    int pat2[2];
    for (int i = 0; i < ALPH; ++i) {
        pat2[0] = pat2[1] = i;
        insert_pattern(pat2, 2);
    }

    /* Forbidden alternating patterns of length 24 for each unordered pair of letters. */
    int pat24[24];
    for (int i = 0; i < ALPH; ++i) {
        for (int j = i + 1; j < ALPH; ++j) {
            for (int k = 0; k < 24; ++k)
                pat24[k] = (k % 2 == 0) ? i : j; /* start with i, alt j */
            insert_pattern(pat24, 24);
            for (int k = 0; k < 24; ++k)
                pat24[k] = (k % 2 == 0) ? j : i; /* start with j, alt i */
            insert_pattern(pat24, 24);
        }
    }

    /* Build failure links using BFS and complete transitions. */
    int queue[MAX_NODES];
    int qh = 0, qt = 0;

    /* Children of root. */
    for (int c = 0; c < ALPH; ++c) {
        int nxt = nodes[0].next[c];
        if (nxt != -1) {
            nodes[nxt].fail = 0;
            queue[qt++] = nxt;
        } else {
            nodes[0].next[c] = 0;  /* missing transition goes to root */
        }
    }

    while (qh < qt) {
        int r = queue[qh++];
        for (int c = 0; c < ALPH; ++c) {
            int u = nodes[r].next[c];
            if (u != -1) {
                nodes[u].fail = nodes[nodes[r].fail].next[c];
                nodes[u].out |= nodes[nodes[u].fail].out;
                queue[qt++] = u;
            } else {
                nodes[r].next[c] = nodes[nodes[r].fail].next[c];
            }
        }
    }

    /* Fill transition table and bad-state flag. */
    for (int s = 0; s < node_count; ++s) {
        bad_state[s] = nodes[s].out;
        for (int c = 0; c < ALPH; ++c)
            go_table[s][c] = nodes[s].next[c];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Build automaton once. */
    if (!initialized) {
        build_automaton();
        initialized = 1;
    }

    /* Handle n == 0 directly. */
    if (n == 0) return (seq_t)1;

    const int S = node_count;          /* number of states */
    static seq_t dp[2][MAX_NODES];     /* DP tables */

    /* Zero initialization of current table. */
    for (int s = 0; s < S; ++s) dp[0][s] = (seq_t)0;
    dp[0][0] = (seq_t)1;               /* start from root */

    int cur = 0, next = 1;

    /* DP over word length. */
    for (size_t len = 0; len < n; ++len) {
        /* Reset next table. */
        for (int s = 0; s < S; ++s) dp[next][s] = (seq_t)0;

        for (int s = 0; s < S; ++s) {
            seq_t val = dp[cur][s];
            if (val == (seq_t)0) continue;
            for (int c = 0; c < ALPH; ++c) {
                int ns = go_table[s][c];
                if (!bad_state[ns]) dp[next][ns] += val;
            }
        }
        /* Swap tables. */
        int tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over all non‑bad states. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < S; ++s) {
        if (!bad_state[s]) result += dp[cur][s];
    }
    return result;
}
