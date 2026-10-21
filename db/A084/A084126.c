/*
 * A084126  Prime factor <= other prime factor of n-th semiprime.
 * https://oeis.org/A084126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lesser prime factor of the n-th semiprime (OEIS A084126)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"  /* provides build_primes */

static idx_t *primes     = NULL;   /* list of primes up to PRIMES_LIMIT */
static size_t primes_cnt = 0;      /* number of generated primes   */

static struct HeapNode {
    idx_t prod;   /* product of primes[i] * primes[j] */
    idx_t i;      /* index of the smaller prime       */
    idx_t j;      /* index of the larger prime        */
} *heap       = NULL;              /* array implementing a min‑heap   */
static size_t heap_size = 0;       /* current number of heap nodes    */

static idx_t *smallpf   = NULL;    /* smaller prime factor of nth semiprime */
static idx_t semip_cnt  = 0;       /* number of generated semiprimes      */
static idx_t semip_cap  = 0;       /* capacity of smallpf array           */

#define PRIMES_LIMIT 200000   /* sufficient for n up to ~10 000 */

/*-------------------- heap helpers --------------------*/
static void heap_push(struct HeapNode node)
{
    if (heap_size == primes_cnt) return;   /* safety guard */
    size_t idx = heap_size++;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].prod <= node.prod) break;
        heap[idx] = heap[parent];
        idx = parent;
    }
    heap[idx] = node;
}

static struct HeapNode heap_pop(void)
{
    struct HeapNode min = heap[0];
    --heap_size;
    if (heap_size == 0) return min;
    heap[0] = heap[heap_size];
    size_t idx = 0;
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < heap_size && heap[left].prod < heap[smallest].prod)
            smallest = left;
        if (right < heap_size && heap[right].prod < heap[smallest].prod)
            smallest = right;
        if (smallest == idx) break;
        struct HeapNode tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx] = tmp;
        idx = smallest;
    }
    return min;
}

static void heapify(void)
{
    for (size_t k = (heap_size / 2); k > 0; --k) {
        size_t idx = k - 1;
        while (1) {
            size_t left  = (idx << 1) + 1;
            size_t right = left + 1;
            size_t smallest = idx;
            if (left < heap_size && heap[left].prod < heap[smallest].prod)
                smallest = left;
            if (right < heap_size && heap[right].prod < heap[smallest].prod)
                smallest = right;
            if (smallest == idx) break;
            struct HeapNode tmp = heap[smallest];
            heap[smallest] = heap[idx];
            heap[idx] = tmp;
            idx = smallest;
        }
    }
}

/*-------------------- allocation helpers --------------------*/
static void ensure_primes(void)
{
    if (primes != NULL) return;          /* already initialised */

    build_primes(PRIMES_LIMIT, &primes, &primes_cnt);

    heap = (struct HeapNode *)malloc(primes_cnt * sizeof(struct HeapNode));
    heap_size = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        struct HeapNode node;
        node.i   = i;
        node.j   = i;
        node.prod = (idx_t)primes[i] * primes[i];
        heap[heap_size++] = node;
    }
    heapify();   /* turn array into a min‑heap */
}

static void ensure_seq_capacity(idx_t needed)
{
    if (semip_cap >= needed) return;
    idx_t new_cap = semip_cap ? semip_cap << 1 : 256;
    while (new_cap < needed) new_cap <<= 1;
    smallpf = (idx_t *)realloc(smallpf, new_cap * sizeof(idx_t));
    semip_cap = new_cap;
}

/*-------------------- generation routine --------------------*/
static void generate_up_to(idx_t n)
{
    ensure_primes();
    while (semip_cnt < n) {
        struct HeapNode node = heap_pop();

        ensure_seq_capacity(semip_cnt + 1);
        smallpf[semip_cnt] = primes[node.i];
        ++semip_cnt;

        node.j++;
        if (node.j < primes_cnt) {
            node.prod = (idx_t)primes[node.i] * primes[node.j];
            heap_push(node);
        }
        /* when node.j >= primes_cnt we simply do not push back onto heap */
    }
}

/*-------------------- public function --------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0; guard */

    if (semip_cnt < n) generate_up_to(n);
    return (seq_t)smallpf[n - 1];
}
