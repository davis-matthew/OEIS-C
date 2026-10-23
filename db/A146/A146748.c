/*
 * A146748  Numbers of the form j^k * k^j, where j,k > 1.
 * https://oeis.org/A146748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   generates sorted values of j^k * k^j for all integers j,k > 1
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 42, u64 n <= 134, u128 n <= 417,
 *            big any n
 * Helpers:   big_pow, seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_from_str.h"

#define MAX_JK 2048   /* upper bound for j and k in the cache */

/* Node in the priority queue, key is log( value ) to avoid overflow in comparisons */
typedef struct {
    seq_t val;     /* exact value of j^k * k^j */
    idx_t j, k;    /* coordinates of the node */
    double logv;   /* k*log(j) + j*log(k) for ordering */
} Node;

/* Marked pairs (j <= k) that have already been inserted into the heap */
static unsigned char visited[MAX_JK + 1][MAX_JK + 1] = {0};

/* Min‑heap implementation for Node */
static Node *heap = NULL;
static size_t heap_len = 0, heap_cap = 0;

static int node_cmp(const Node *a, const Node *b) {
    if (a->logv < b->logv) return -1;
    if (a->logv > b->logv) return 1;
    /* deterministic tie‑break */
    if (a->j != b->j) return (a->j < b->j) ? -1 : 1;
    if (a->k != b->k) return (a->k < b->k) ? -1 : 1;
    return 0;
}

static void heap_swap(Node *a, Node *b) {
    Node t = *a; *a = *b; *b = t;
}

static void heapify_up(size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (node_cmp(&heap[idx], &heap[parent]) >= 0) break;
        heap_swap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

static void heapify_down(size_t idx) {
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_len) break;
        size_t right = left + 1;
        size_t smaller = left;
        if (right < heap_len && node_cmp(&heap[right], &heap[left]) < 0)
            smaller = right;
        if (node_cmp(&heap[smaller], &heap[idx]) >= 0) break;
        heap_swap(&heap[smaller], &heap[idx]);
        idx = smaller;
    }
}

static void push_node(const Node n) {
    if (heap_len == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 256;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    heap[heap_len++] = n;
    heapify_up(heap_len - 1);
}

static Node pop_node(void) {
    Node min = heap[0];
    heap[0] = heap[--heap_len];
    heapify_down(0);
    return min;
}

/* Cache of produced values */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Build the cache up to the nth term (1‑based) */
static void build_until(idx_t n) {
    /* initial node (2,2) */
    if (heap_len == 0) {
        seq_t v = seq_mul(big_pow((seq_t)2, (seq_t)2),
                          big_pow((seq_t)2, (seq_t)2));
        Node start = { v, 2, 2,
                      2 * log((double)2) + 2 * log((double)2) };
        push_node(start);
        visited[2][2] = 1;
    }

    if (cache_len == 0) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        Node cur = pop_node();

        /* deduplicate */
        if (cache_len == 0 || cache[cache_len - 1] != cur.val) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur.val;
        }

        /* successor (j, k+1) */
        if (cur.k + 1 <= MAX_JK && !visited[cur.j][cur.k + 1]) {
            visited[cur.j][cur.k + 1] = 1;
            seq_t v1 = seq_mul(big_pow((seq_t)cur.j, (seq_t)(cur.k + 1)),
                               big_pow((seq_t)(cur.k + 1), (seq_t)cur.j));
            Node n1 = { v1, cur.j, cur.k + 1,
                        cur.j * log((double)(cur.k + 1))
                        + (cur.k + 1) * log((double)cur.j) };
            push_node(n1);
        }

        /* successor (j+1, k) only if canonical (j+1 <= k) */
        if (cur.j + 1 <= cur.k && cur.j + 1 <= MAX_JK &&
            !visited[cur.j + 1][cur.k]) {
            visited[cur.j + 1][cur.k] = 1;
            seq_t v2 = seq_mul(big_pow((seq_t)(cur.j + 1), (seq_t)cur.k),
                               big_pow((seq_t)cur.k, (seq_t)(cur.j + 1)));
            Node n2 = { v2, cur.j + 1, cur.k,
                        (cur.j + 1) * log((double)cur.k)
                        + cur.k * log((double)(cur.j + 1)) };
            push_node(n2);
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return seq_from_str("0");   /* offset is 1 */
    build_until(n);
    return cache[n - 1];
}
