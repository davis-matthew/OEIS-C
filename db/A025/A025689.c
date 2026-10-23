/*
 * A025689  Exponent of 10 (value of j) in n-th number of form 7^i*10^j.
 * https://oeis.org/A025689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) - exponent j in the n-th smallest number of the form * 7^i
 *            * 10^j, sorted by the increasing value of the product. Generated
 *            * via a min‑heap; no hard‑coded table
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* compile‑time key for the hash table (i<<32)|j */
#define HKEY(i, j) (((uint64_t)(i) << 32) | (uint64_t)(j))

/* ------------------- heap node --------------------------------- */
typedef struct {
    double key;        /* i*log10(7)+j  (used for ordering) */
    idx_t i;
    idx_t j;
} HeapNode;

/* ------------------- global storage ---------------------------- */
static HeapNode *heap = NULL;
static size_t heap_size = 0;
static size_t heap_capacity = 0;

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_count = 0;

/* simple open‑address hash set for visited (i,j) pairs */
#define HASH_SIZE 65536           /* power of two, > 2*n for n≤10000 */
static uint64_t *hash_keys = NULL;
static uint8_t  *hash_used = NULL;
static bool hash_initialized = false;

/* ------------------- log10(7) ------------------------------- */
static const double LOG10_7 = 0.8450980400142568;   /* log10(7) */

/* ------------------- heap helpers ---------------------------- */
static int cmp_nodes(const HeapNode *a, const HeapNode *b)
{
    if (a->key < b->key) return -1;
    if (a->key > b->key) return 1;
    /* Ties are impossible for distinct (i,j) but keep deterministic */
    if (a->i < b->i) return -1;
    if (a->i > b->i) return 1;
    if (a->j < b->j) return -1;
    if (a->j > b->j) return 1;
    return 0;
}

static void heap_push(const HeapNode *n)
{
    if (heap_size == heap_capacity) {
        size_t new_cap = heap_capacity ? heap_capacity * 2 : 64;
        HeapNode *tmp = (HeapNode *)realloc(heap, new_cap * sizeof(HeapNode));
        if (!tmp) exit(1);
        heap = tmp;
        heap_capacity = new_cap;
    }
    size_t idx = heap_size++;
    heap[idx] = *n;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (cmp_nodes(&heap[idx], &heap[parent]) >= 0) break;
        HeapNode tmp = heap[idx];
        heap[idx] = heap[parent];
        heap[parent] = tmp;
        idx = parent;
    }
}

static HeapNode heap_pop(void)
{
    HeapNode top = heap[0];
    heap_size--;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t idx = 0;
        while (1) {
            size_t left = idx * 2 + 1;
            size_t right = left + 1;
            size_t best = idx;
            if (left < heap_size && cmp_nodes(&heap[left], &heap[best]) < 0)
                best = left;
            if (right < heap_size && cmp_nodes(&heap[right], &heap[best]) < 0)
                best = right;
            if (best == idx) break;
            HeapNode tmp = heap[idx];
            heap[idx] = heap[best];
            heap[best] = tmp;
            idx = best;
        }
    }
    return top;
}

/* ------------------- hash helpers -------------------------------- */
static void init_hash(void)
{
    hash_keys = (uint64_t *)malloc(HASH_SIZE * sizeof(uint64_t));
    hash_used = (uint8_t *)calloc(HASH_SIZE, 1);
    if (!hash_keys || !hash_used) exit(1);
    hash_initialized = true;
}

static bool visited_insert(idx_t i, idx_t j)
{
    if (!hash_initialized) init_hash();
    uint64_t key = HKEY(i, j);
    size_t h = key & (HASH_SIZE - 1);
    while (hash_used[h]) {
        if (hash_keys[h] == key) return false;   /* already present */
        h = (h + 1) & (HASH_SIZE - 1);
    }
    hash_used[h] = 1;
    hash_keys[h] = key;
    return true;   /* inserted */
}

/* ------------------- main function ----------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, return 0 */

    /* init cache & heap on first call */
    if (!cache) {
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) exit(1);
        /* start with (0,0) */
        HeapNode first = {0.0, 0, 0};
        heap_push(&first);
        cache_count = 0;
    }

    /* grow cache if needed */
    if ((size_t)n > cache_capacity) {
        size_t new_cap = cache_capacity;
        while ((size_t)n > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* generate terms until we reach n */
    while (cache_count < n) {
        HeapNode cur = heap_pop();
        cache[cache_count++] = (seq_t)(cur.j);

        /* push successors if not visited */
        if (visited_insert(cur.i + 1, cur.j)) {
            HeapNode child = {cur.key + LOG10_7, cur.i + 1, cur.j};
            heap_push(&child);
        }
        if (visited_insert(cur.i, cur.j + 1)) {
            HeapNode child = {cur.key + 1.0, cur.i, cur.j + 1};
            heap_push(&child);
        }
    }

    return cache[n - 1];
}
