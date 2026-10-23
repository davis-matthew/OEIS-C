/*
 * A138990  a(n) = Frobenius number for 4 successive primes = F[p(n), p(n+1),
 *          p(n+2), p(n+3)].
 * https://oeis.org/A138990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Frobenius number of the 4 consecutive primes starting
 *            with the n-th prime (1‑based). The algorithm implements Dijkstra
 *            on the residues modulo the smallest of the four primes
 * Range:     u8 n <= 10, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_arr_alloc, seq_arr_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_arr_alloc.h"
#include "../utils/seq_arr_free.h"

/* ---------- simple sieve to obtain the first primes needed ---------- */
static bool primes_inited = false;
static uint64_t PRIMES[11000];
static size_t PRIME_COUNT = 0;

static void init_primes(void) {
    if (primes_inited) return;
    const size_t LIMIT = 200000;                     /* enough for >10000 primes */
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    size_t count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (count < 11000) PRIMES[count++] = (uint64_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    PRIME_COUNT = count;
    free(is_comp);
    primes_inited = true;
}

/* ---------- simple binary min‑heap for Dijkstra ---------- */
typedef struct {
    seq_t dist;
    idx_t res;
} heap_node_t;

typedef struct {
    heap_node_t *data;
    size_t size;
    size_t capacity;
} min_heap_t;

static void heap_init(min_heap_t *h, size_t capacity) {
    h->data = (heap_node_t*)calloc(capacity, sizeof(heap_node_t));
    h->size = 0;
    h->capacity = capacity;
}

static void heap_destroy(min_heap_t *h) {
    free(h->data);
}

static void heap_push(min_heap_t *h, heap_node_t node) {
    size_t i = h->size++;
    if (i >= h->capacity) {          /* grow */
        size_t newcap = h->capacity * 2 + 1;
        heap_node_t *tmp = (heap_node_t*)realloc(h->data, newcap * sizeof(heap_node_t));
        h->data = tmp;
        h->capacity = newcap;
    }
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h->data[parent].dist <= node.dist) break;
        h->data[i] = h->data[parent];
        i = parent;
    }
    h->data[i] = node;
}

static bool heap_empty(const min_heap_t *h) {
    return h->size == 0;
}

static heap_node_t heap_pop(min_heap_t *h) {
    heap_node_t res = h->data[0];
    size_t i = 0;
    size_t last = --h->size;
    if (last > 0) {
        heap_node_t t = h->data[last];
        while (1) {
            size_t left = (i << 1) + 1;
            size_t right = left + 1;
            size_t smallest = left;
            if (right <= last && h->data[right].dist < h->data[left].dist)
                smallest = right;
            if (smallest > last || t.dist <= h->data[smallest].dist) break;
            h->data[i] = h->data[smallest];
            i = smallest;
        }
        h->data[i] = t;
    }
    return res;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    init_primes();
    if (n == 0 || n + 3 >= PRIME_COUNT) return 0;  /* undefined for very small n */

    const uint64_t p0 = PRIMES[n - 1];
    const uint64_t p1 = PRIMES[n];
    const uint64_t p2 = PRIMES[n + 1];
    const uint64_t p3 = PRIMES[n + 2];
    const idx_t mod = (idx_t)p0;            /* modulus for residues */

    bool *visited = (bool*)calloc(mod, sizeof(bool));
    seq_t *dist = seq_arr_alloc(mod);
    if (!visited || !dist) {
        free(visited);
        seq_arr_free(dist);
        return 0;
    }

    min_heap_t heap;
    heap_init(&heap, mod);
    heap_push(&heap, (heap_node_t){ .dist = (seq_t)0, .res = 0 });

    seq_t coins[3] = { (seq_t)p1, (seq_t)p2, (seq_t)p3 };

    while (!heap_empty(&heap)) {
        heap_node_t cur = heap_pop(&heap);
        idx_t r = cur.res;
        if (visited[r]) continue;
        visited[r] = true;
        dist[r] = cur.dist;

        for (int i = 0; i < 3; ++i) {
            idx_t r2 = (r + (idx_t)coins[i]) % mod;
            seq_t d2 = cur.dist + coins[i];
            if (!visited[r2])
                heap_push(&heap, (heap_node_t){ .dist = d2, .res = r2 });
        }
    }

    seq_t best = (seq_t)0;
    for (idx_t r = 1; r < mod; ++r) {
        if (dist[r] != (seq_t)0) {   /* reachable residue */
            seq_t val = dist[r] - (seq_t)p0;
            if (val > best) best = val;
        }
    }

    free(visited);
    seq_arr_free(dist);
    heap_destroy(&heap);
    return best;
}
