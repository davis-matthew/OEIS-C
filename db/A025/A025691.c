/*
 * A025691  Exponent of 10 (value of j) in n-th number of form 9^i*10^j.
 * https://oeis.org/A025691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = exponent of 10 in the n‑th smallest number of the form 9^i·10^j
    (i,j ≥ 0).  The numbers 9^i·10^j are generated in increasing order by
    a Dijkstra / priority‑queue algorithm on the lattice (i,j); the key is
    i·log₁₀(9)+j.  The function returns the j part of the n‑th element.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_pow2.h"

typedef struct {
    idx_t i;
    idx_t j;
    double key;
} Node;

/* ---------- heap helpers ---------- */
static void heap_init(Node **data, size_t *cap, size_t *size)
{
    *cap  = 4;
    *size = 0;
    *data = (Node *)malloc(*cap * sizeof(Node));
}

static void heap_free(Node *data) { free(data); }

static void heap_push(Node **data, size_t *cap, size_t *size, Node v)
{
    if (*size == *cap) {
        *cap <<= 1;
        *data = (Node *)realloc(*data, *cap * sizeof(Node));
    }
    size_t pos = (*size)++;
    while (pos) {
        size_t parent = (pos - 1) >> 1;
        if (v.key >= (*data)[parent].key) break;
        (*data)[pos] = (*data)[parent];
        pos = parent;
    }
    (*data)[pos] = v;
}

static Node heap_pop(Node *data, size_t *size)
{
    Node top = data[0];
    (*size)--;
    Node last = data[*size];
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < *size && data[left].key < last.key) smallest = left;
        if (right < *size && data[right].key < data[smallest].key) smallest = right;
        if (smallest == pos) break;
        data[pos] = data[smallest];
        pos = smallest;
    }
    data[pos] = last;
    return top;
}

/* ---------- hash set for visited pairs ---------- */
static inline uint64_t pair_key(idx_t i, idx_t j)
{
    return (((uint64_t)i) << 32) | (uint64_t)j;
}

static bool visited_insert(uint64_t key, uint64_t *keys, uint8_t *used, size_t cap)
{
    size_t idx = (size_t)((key * 11400714785074694791ULL) & (cap - 1));
    while (used[idx]) {
        if (keys[idx] == key) return false;      /* already present */
        idx = (idx + 1) & (cap - 1);
    }
    used[idx] = 1;
    keys[idx] = key;
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Special case: the very first number 9^0·10^0 has j = 0. */
    if (n <= 1) return (seq_t)0;

    /* Big enough to cover all required pairs for n up to ~10000 */
    size_t visit_cap = next_pow2((size_t)n * 4 + 16);
    uint64_t *visit_keys  = (uint64_t *)calloc(visit_cap, sizeof(uint64_t));
    uint8_t  *visit_used  = (uint8_t *)calloc(visit_cap, sizeof(uint8_t));

    /* Heap for the sorted pairs */
    Node *heap_data; size_t heap_cap, heap_size;
    heap_init(&heap_data, &heap_cap, &heap_size);

    /* Start from (0,0) */
    Node start = {0, 0, 0.0};
    heap_push(&heap_data, &heap_cap, &heap_size, start);
    visited_insert(pair_key(0,0), visit_keys, visit_used, visit_cap);

    const double LOG9 = 0.954242509439324960807318593156;  /* log10(9) */

    seq_t result = 0;
    for (idx_t cnt = 1; cnt <= n; ++cnt) {
        Node cur = heap_pop(heap_data, &heap_size);
        if (cnt == n) {
            result = (seq_t)cur.j;
            break;
        }

        /* child (i+1, j) : multiply by 9 */
        uint64_t k1 = pair_key(cur.i + 1, cur.j);
        if (visited_insert(k1, visit_keys, visit_used, visit_cap)) {
            Node child1 = {cur.i + 1, cur.j, cur.key + LOG9};
            heap_push(&heap_data, &heap_cap, &heap_size, child1);
        }

        /* child (i, j+1) : multiply by 10 */
        uint64_t k2 = pair_key(cur.i, cur.j + 1);
        if (visited_insert(k2, visit_keys, visit_used, visit_cap)) {
            Node child2 = {cur.i, cur.j + 1, cur.key + 1.0};
            heap_push(&heap_data, &heap_cap, &heap_size, child2);
        }
    }

    heap_free(heap_data);
    free(visit_keys);
    free(visit_used);

    return result;
}
