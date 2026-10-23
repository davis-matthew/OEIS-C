/*
 * A157941  Numbers n divisible by the largest prime < sqrt(n),
 *          A007917(A000196(n-1)).
 * https://oeis.org/A157941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer n such that n is divisible by the largest
 *            prime < sqrt(n) (OEIS A157941)
 * Range:     u8 n <= 31, u16 n <= 515, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Node for the min‑heap: one prime window */
typedef struct Node {
    uint64_t prime;        /* current prime p */
    uint64_t next_prime;   /* next prime q (>p) */
    uint64_t current_k;    /* current multiplier k */
    uint64_t k_max;        /* maximum allowed k */
    seq_t value;           /* current candidate p*k */
} Node;

/* -------------------- heap ---------------------------------- */
static Node **heap = NULL;
static size_t heap_cap = 0, heap_size = 0;

static void heap_expand(void) {
    size_t new_cap = heap_cap ? heap_cap << 1 : 16;
    Node **tmp = (Node**)realloc(heap, new_cap * sizeof(Node*));
    if (!tmp) abort();
    heap = tmp;
    heap_cap = new_cap;
}

static void heap_push(Node *n) {
    if (heap_size == heap_cap) heap_expand();
    size_t i = heap_size++;
    heap[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p]->value <= heap[i]->value) break;
        Node *tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static Node *heap_pop(void) {
    Node *top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (true) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right]->value < heap[left]->value)
            smallest = right;
        if (heap[i]->value <= heap[smallest]->value) break;
        Node *tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return top;
}

/* -------------------- sequence storage --------------------- */
static seq_t *seq = NULL;
static idx_t seq_cap = 0, seq_len = 0;

/* last prime added to the heap */
static uint64_t last_prime = 0;

/* add next prime p to the heap */
static void add_next_prime(void) {
    uint64_t p = (last_prime == 0) ? 2 : next_prime(last_prime);
    uint64_t q = next_prime(p);     /* first prime > p */
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) abort();
    node->prime = p;
    node->next_prime = q;
    node->current_k = p + 1;
    unsigned __int128 qq = (unsigned __int128)q * q;
    node->k_max = (uint64_t)((qq - 1) / p);
    node->value = (seq_t)((unsigned __int128)p * node->current_k);
    heap_push(node);
    last_prime = p;
}

/* -------------------- main sequence function --------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety guard */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        /* initialise the heap when empty */
        if (heap_size == 0) add_next_prime();

        /* ensure seq has room for the new term */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
            seq_cap = new_cap;
        }

        /* extract smallest candidate */
        Node *node = heap_pop();
        seq[seq_len++] = node->value;

        /* advance this node */
        node->current_k++;
        if (node->current_k <= node->k_max) {
            node->value = (seq_t)((unsigned __int128)node->prime * node->current_k);
            heap_push(node);
        }
        /* otherwise the node is finished and discarded */
    }

    return seq[n - 1];
}
