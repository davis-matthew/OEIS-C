/*
 * A084383  a(0)=0; for n>0, a(n) = smallest number that is not a
 *          concatenation of any number of distinct earlier terms in
 *          increasing order.
 * https://oeis.org/A084383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084383: a(0)=0; for n>0, a(n) is the smallest number that cannot be expressed as a concatenation of any number of distinct earlier terms in increasing order. */
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the trie node structure. */
typedef struct TrieNode {
    int child[10];
    bool is_end;
    seq_t val;
} TrieNode;

/* Trie table and related state. */
#define MAX_TRIE_NODES 200000
static TrieNode trie[MAX_TRIE_NODES];
static size_t trie_nodes = 0;

/* Sequence storage and state. */
#define MAX_SEQ 20000
static seq_t seq_arr[MAX_SEQ];
static size_t seq_computed = 0;

/* Helper to convert an unsigned integer of type seq_t to a decimal string. */
static void seq_to_str(seq_t x, char *out, int *len) {
    char tmp[64];
    int pos = 0;

    if (x == 0) {
        tmp[pos++] = '0';
    } else {
        seq_t t = x;
        char rev[64];
        int revpos = 0;
        while (t) {
            int d = (int)(t % 10);
            rev[revpos++] = '0' + d;
            t /= 10;
        }
        for (int i = revpos - 1; i >= 0; --i) {
            tmp[pos++] = rev[i];
        }
    }

    *len = pos;
    for (int i = 0; i < pos; ++i) {
        out[i] = tmp[i];
    }
}

/* Trie utility: initialize a node. */
static void trie_init_node(size_t idx) {
    for (int i = 0; i < 10; ++i)
        trie[idx].child[i] = -1;
    trie[idx].is_end = false;
    trie[idx].val = 0;
}

/* Insert a term into the trie. */
static void trie_insert(seq_t val) {
    char buf[64];
    int len;
    seq_to_str(val, buf, &len);

    size_t node = 0; /* root */
    for (int i = 0; i < len; ++i) {
        int d = buf[i] - '0';
        if (trie[node].child[d] == -1) {
            size_t newnode = ++trie_nodes;
            trie_init_node(newnode);
            trie[node].child[d] = newnode;
        }
        node = trie[node].child[d];
    }
    trie[node].is_end = true;
    trie[node].val = val;
}

/* Global parameters used by the DFS. */
static const char *g_s;  /* decimal string of the candidate */
static int g_len;        /* length of g_s */

/* Depth‑first search to test representability. */
static bool dfs_concat(int pos, seq_t last) {
    if (pos == g_len) return true;          /* reached the end */

    size_t node = 0;                        /* start at root */
    for (int i = pos; i < g_len; ++i) {
        int d = g_s[i] - '0';
        node = trie[node].child[d];
        if (node == (size_t)-1) break;      /* no further match */

        if (trie[node].is_end) {
            seq_t val = trie[node].val;
            if (last == ((seq_t)-1) || val > last) {
                if (dfs_concat(i + 1, val))
                    return true;
            }
        }
    }
    return false;
}

/* Test whether a candidate number can be concatenated from earlier terms. */
static bool can_be_concatenated(seq_t cand) {
    char buf[64];
    int len;
    seq_to_str(cand, buf, &len);
    g_s = buf;
    g_len = len;
    return dfs_concat(0, (seq_t)-1);
}

/* Main function required by the framework. */
seq_t a(idx_t n) {
    /* One‑time initialization. */
    static bool initialized = false;
    if (!initialized) {
        trie_init_node(0);                   /* root node */
        trie_nodes = 0;
        seq_arr[0] = 0;
        trie_insert(0);
        seq_computed = 1;
        initialized = true;
    }

    /* Extend the sequence if necessary. */
    if (n >= seq_computed) {
        if (n >= MAX_SEQ) {
            /* Sequence buffer overflow – should not happen for tested ranges. */
            return 0;
        }
        for (size_t i = seq_computed; i <= (size_t)n; ++i) {
            seq_t cand = seq_arr[i - 1] + 1;
            while (true) {
                if (!can_be_concatenated(cand)) {
                    seq_arr[i] = cand;
                    trie_insert(cand);
                    break;
                }
                ++cand;
            }
        }
        seq_computed = n + 1;
    }

    return seq_arr[(size_t)n];
}
