/*
 * A191895  Smallest multiple of n whose factorial digit sum equals n.
 * https://oeis.org/A191895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive integer m with m mod n = 0 and *
 *            sum_{digits d of m} d! = n
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Node for BFS */
typedef struct {
    uint32_t sum;      /* current factorial digit sum (≤ n) */
    uint32_t rem;      /* current value mod n (0 … n-1) */
    uint32_t parent;   /* index of parent node, UINT32_MAX for root */
    uint8_t  digit;    /* digit that produced this node */
} Node;

static inline bool bit_get(const uint8_t *bits, uint64_t idx)
{
    return (bits[idx >> 3] >> (idx & 7u)) & 1u;
}
static inline void bit_set(uint8_t *bits, uint64_t idx)
{
    bits[idx >> 3] |= (1u << (idx & 7u));
}

seq_t a(idx_t n)
{
    /* trivial for n == 0 (not an official case) */
    if (n == 0) return seq_from_str("0");

    /* factorial for decimal digits 0! … 9! */
    static const int fact[10] = {1,1,2,6,24,120,720,5040,40320,362880};

    uint64_t n64 = (uint64_t)n;
    uint64_t total_states = (n64 + 1ULL) * n64;          /* sum 0..n, rem 0..n-1 */
    size_t vis_bytes = (total_states + 7ULL) >> 3ULL;
    uint8_t *visited = (uint8_t *)calloc(vis_bytes, 1);
    if (!visited) return seq_from_str("0");

    /* dynamic arrays for nodes and queue */
    size_t nodes_cap = 1024;
    Node *nodes = (Node *)malloc(nodes_cap * sizeof(Node));
    size_t nodes_len = 0;

    size_t q_cap = 1024;
    uint32_t *queue = (uint32_t *)malloc(q_cap * sizeof(uint32_t));
    size_t q_head = 0, q_tail = 0;

    /* helper macros for resizing */
    #define GROW_NODE_ARRAY() do { \
        nodes_cap *= 2; \
        nodes = (Node *)realloc(nodes, nodes_cap * sizeof(Node)); \
    } while (0)
    #define GROW_QUEUE_ARRAY() do { \
        q_cap *= 2; \
        queue = (uint32_t *)realloc(queue, q_cap * sizeof(uint32_t)); \
    } while (0)

    /* enqueue all one‑digit starting nodes (no leading zero) */
    for (int d = 1; d <= 9; ++d) {
        uint32_t s = (uint32_t)fact[d];
        if (s > (uint32_t)n) continue;
        uint32_t r = (uint32_t)(d % n);
        uint64_t idx = ((uint64_t)s) * n64 + r;
        if (bit_get(visited, idx)) continue;
        bit_set(visited, idx);
        if (nodes_len == nodes_cap) GROW_NODE_ARRAY();
        nodes[nodes_len].sum = s;
        nodes[nodes_len].rem = r;
        nodes[nodes_len].parent = UINT32_MAX;
        nodes[nodes_len].digit = (uint8_t)d;
        if (q_tail == q_cap) GROW_QUEUE_ARRAY();
        queue[q_tail++] = (uint32_t)nodes_len;
        ++nodes_len;
    }

    uint32_t target_idx = 0;
    bool found = false;

    while (q_head < q_tail) {
        uint32_t curIdx = queue[q_head++];
        Node cur = nodes[curIdx];

        if (cur.sum == (uint32_t)n && cur.rem == 0) {
            target_idx = curIdx;
            found = true;
            break;
        }

        for (int d = 0; d <= 9; ++d) {
            uint32_t newSum = cur.sum + (uint32_t)fact[d];
            if (newSum > (uint32_t)n) continue;
            uint32_t newRem = (uint32_t)(((uint64_t)cur.rem * 10u + (uint32_t)d) % n);
            uint64_t idx = ((uint64_t)newSum) * n64 + newRem;
            if (bit_get(visited, idx)) continue;
            bit_set(visited, idx);

            if (nodes_len == nodes_cap) GROW_NODE_ARRAY();
            nodes[nodes_len].sum = newSum;
            nodes[nodes_len].rem = newRem;
            nodes[nodes_len].parent = curIdx;
            nodes[nodes_len].digit = (uint8_t)d;

            if (q_tail == q_cap) GROW_QUEUE_ARRAY();
            queue[q_tail++] = (uint32_t)nodes_len;
            ++nodes_len;
        }
    }

    seq_t result = seq_from_str("0");
    if (found) {
        /* reconstruct number */
        size_t len = 0;
        uint32_t p = target_idx;
        while (p != UINT32_MAX) {
            ++len;
            p = nodes[p].parent;
        }

        char *buf = (char *)malloc(len + 1);
        p = target_idx;
        for (size_t i = 0; i < len; ++i) {
            buf[len - 1 - i] = '0' + (char)nodes[p].digit;
            p = nodes[p].parent;
        }
        buf[len] = '\0';
        result = seq_from_str(buf);
        free(buf);
    }

    free(visited);
    free(nodes);
    free(queue);
    return result;
}
