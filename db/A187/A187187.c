/*
 * A187187  Parse the infinite string 0123456780123456780123456780... into
 *          distinct phrases 0, 1, 2, 3, 4, 5, 6, 7, 8, 01, 23, 45, 67, 80,
 *          12, 34, 56, 78, 012, ...; a(n) = length of n-th phrase.
 * https://oeis.org/A187187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = length of the n‑th distinct phrase obtained by parsing
 *            the infinite string 012345678012345678... greedily.
 *            Implementation uses a trie over digits 0–8; each node contains
 *            an “end” flag marking whether that prefix has already been used
 *            as a phrase. The function runs in O(n) time with modest...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct Node {
    int child[9];
    bool end;
};

static inline void node_init(struct Node *n)
{
    for (int i = 0; i < 9; ++i) n->child[i] = -1;
    n->end = false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 */

    /* allocate space for at most n phrases plus the root node */
    size_t cap   = (size_t)n + 1;
    struct Node *nodes = (struct Node*)malloc(cap * sizeof *nodes);
    if (!nodes) return 0;                /* allocation failure */

    /* initialise all trie nodes */
    for (size_t i = 0; i < cap; ++i) node_init(&nodes[i]);

    size_t used = 1;                     /* next free node index (root = 0) */
    uint64_t pos = 0;                    /* current position in the infinite string */

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t cur = pos;              /* start looking for the i‑th phrase */
        int node = 0;                    /* start at root */

        while (1) {
            int d = (int)(cur % 9);       /* digit at position cur (0-8) */
            int child = nodes[node].child[d];

            if (child == -1) {            /* missing branch => new phrase */
                /* grow trie array if necessary */
                if (used == cap) {
                    cap <<= 1;
                    struct Node *tmp = (struct Node*)realloc(nodes, cap * sizeof *nodes);
                    if (!tmp) { free(nodes); return 0; }
                    nodes = tmp;
                }
                int new_idx = (int)used++;
                nodes[node].child[d] = new_idx;
                node_init(&nodes[new_idx]);

                node = new_idx;
                nodes[node].end = true;

                seq_t len = (seq_t)(cur - pos + 1);
                pos = cur + 1;
                if (i == n) { free(nodes); return len; }
                break;
            } else {                       /* branch exists */
                node = child;
                if (!nodes[node].end) {      /* first time this prefix is used */
                    nodes[node].end = true;
                    seq_t len = (seq_t)(cur - pos + 1);
                    pos = cur + 1;
                    if (i == n) { free(nodes); return len; }
                    break;
                }
                /* prefix already used; try longer one */
                ++cur;
            }
        }
    }

    /* should never reach here */
    free(nodes);
    return 0;
}
