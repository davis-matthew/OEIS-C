/*
 * A168929  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168929: number of reduced words of length n in the Coxeter group
    on 12 generators S_i with relations S_i^2 = (S_i S_j)^22 = I.
    The sequence counts words over a 12‑letter alphabet that avoid the
    forbidden patterns "aa" (adjacent equal letters) and the 22‑letter
    alternations ijij… for each unordered pair {i,j}.  An
    Aho–Corasick automaton is built once, and a simple DP over its
    states gives the desired numbers.  Results are memoised so that any
    ordering of calls is O(n) after the first call.  */
 
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* alphabet size – must be a constant expression */
enum { ALPHABET = 12 };

/* Automaton node */
typedef struct {
    int next[ALPHABET];   /* transitions */
    int fail;             /* failure link */
    unsigned char bad;    /* 1 if this state corresponds to a forbidden word prefix */
} Node;

/* static data */
static int   automaton_built = 0;
static size_t state_count = 0;
static Node *nodes = NULL;

static seq_t *dp_prev = NULL;
static seq_t *dp_cur  = NULL;
static seq_t *memo    = NULL;
static size_t memo_cap = 0;
static size_t memo_len = 0;
static idx_t  max_computed = 0;

/* ----------------------------------------------------------------- */
/*  Build the automaton once                                         */
/* ----------------------------------------------------------------- */
static void build_automaton(void)
{
    /* -----------------------------------------------------------------
       1.  Create nodes
       ----------------------------------------------------------------- */
    size_t node_cap  = 64;
    size_t node_cnt  = 1;        /* root is node 0 */
    nodes = (Node *)malloc(node_cap * sizeof(Node));
    for (int c = 0; c < ALPHABET; ++c) nodes[0].next[c] = -1;
    nodes[0].fail = 0;
    nodes[0].bad  = 0;

    #define NEW_NODE() \
        do { \
            if (node_cnt == node_cap) { \
                node_cap <<= 1; \
                nodes = (Node *)realloc(nodes, node_cap * sizeof(Node)); \
            } \
            int id = node_cnt++; \
            for (int c = 0; c < ALPHABET; ++c) nodes[id].next[c] = -1; \
            nodes[id].fail = 0; \
            nodes[id].bad  = 0; \
        } while (0)

    /* (a) forbid "aa" */
    for (int a = 0; a < ALPHABET; ++a) {
        int cur = 0;
        if (nodes[cur].next[a] == -1) {
            NEW_NODE();
            nodes[cur].next[a] = node_cnt - 1;
        }
        cur = nodes[cur].next[a];
        if (nodes[cur].next[a] == -1) {
            NEW_NODE();
            nodes[cur].next[a] = node_cnt - 1;
        }
        cur = nodes[cur].next[a];
        nodes[cur].bad = 1;
    }

    /* (b) forbid 22‑letter alternations ijij… for each i!=j */
    for (int i = 0; i < ALPHABET; ++i) {
        for (int j = 0; j < ALPHABET; ++j) if (i != j) {
            int cur = 0;
            for (int k = 0; k < 22; ++k) {
                int c = (k & 1) ? j : i;
                if (nodes[cur].next[c] == -1) {
                    NEW_NODE();
                    nodes[cur].next[c] = node_cnt - 1;
                }
                cur = nodes[cur].next[c];
            }
            nodes[cur].bad = 1;
        }
    }

    /* -----------------------------------------------------------------
       2.  Build failure links (Aho–Corasick)
       ----------------------------------------------------------------- */
    int *queue = (int *)malloc(node_cnt * sizeof(int));
    size_t q_head = 0, q_tail = 0;

    /* Initialise children of root */
    for (int c = 0; c < ALPHABET; ++c) {
        int nxt = nodes[0].next[c];
        if (nxt != -1) {
            nodes[nxt].fail = 0;
            queue[q_tail++] = nxt;
        } else {
            nodes[0].next[c] = 0;
        }
    }

    while (q_head < q_tail) {
        int r   = queue[q_head++];
        int fr  = nodes[r].fail;
        nodes[r].bad |= nodes[fr].bad;
        for (int c = 0; c < ALPHABET; ++c) {
            int nxt = nodes[r].next[c];
            if (nxt != -1) {
                nodes[nxt].fail = nodes[fr].next[c];
                queue[q_tail++] = nxt;
            } else {
                nodes[r].next[c] = nodes[fr].next[c];
            }
        }
    }
    free(queue);

    state_count = node_cnt;

    /* -----------------------------------------------------------------
       3.  Allocate DP arrays
       ----------------------------------------------------------------- */
    dp_prev = (seq_t *)calloc(state_count, sizeof(seq_t));
    dp_cur  = (seq_t *)calloc(state_count, sizeof(seq_t));
    dp_prev[0] = 1;   /* empty word */

    /* -----------------------------------------------------------------
       4.  Allocate memoisation storage
       ----------------------------------------------------------------- */
    memo_cap = 16;
    memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    memo[0] = 1;  /* a(0) = 1 */
    memo_len = 1;

    max_computed = 0;
    automaton_built = 1;
    #undef NEW_NODE
}

/* --------------------------------------------------------------------- */
/*  Query function                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!automaton_built) {
        build_automaton();
    }

    /* Extend memoisation up to n */
    if (n > max_computed) {
        for (idx_t len = max_computed; len < n; ++len) {
            /* reset dp_cur to zero */
            for (size_t s = 0; s < state_count; ++s) dp_cur[s] = 0;

            /* transitions from words of length len */
            for (size_t s = 0; s < state_count; ++s) {
                seq_t val = dp_prev[s];
                if (!val) continue;
                if (nodes[s].bad) continue;  /* safety: should never happen */
                for (int c = 0; c < ALPHABET; ++c) {
                    int t = nodes[s].next[c];
                    if (!nodes[t].bad) {
                        dp_cur[t] += val;
                    }
                }
            }

            /* sum over all non‑bad states → a(len+1) */
            seq_t total = 0;
            for (size_t s = 0; s < state_count; ++s) {
                if (!nodes[s].bad) total += dp_cur[s];
            }

            /* store result */
            if (memo_len >= memo_cap) {
                memo_cap <<= 1;
                memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
            }
            memo[memo_len++] = total;

            /* prepare for next iteration */
            seq_t *tmp = dp_prev;
            dp_prev = dp_cur;
            dp_cur  = tmp;
        }

        max_computed = n;
    }

    return memo[n];
}
