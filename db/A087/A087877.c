/*
 * A087877  Primes of the form (4n+1)^2 + (4m)^2, m,n = 0,1,2..
 * https://oeis.org/A087877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of the form (4x+1)^2 + (4y)^2, x,y≥0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

typedef struct { seq_t val; idx_t n; idx_t m; } Node;

typedef struct {
    Node *arr;
    size_t sz;
    size_t cap;
} Heap;

static Heap heap = {NULL, 0, 0};
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static bool initialized = false;

/* ---------- Heap operations ---------- */
static void heap_init(size_t cap) {
    heap.arr = (Node*)malloc(cap * sizeof(Node));
    heap.cap = cap;
    heap.sz  = 0;
}

static void heap_push(const Node *nd) {
    if (heap.sz + 1 > heap.cap) {
        size_t newcap = (heap.cap? heap.cap * 2 : 16);
        heap.arr = (Node*)realloc(heap.arr, newcap * sizeof(Node));
        heap.cap = newcap;
    }
    heap.arr[heap.sz++] = *nd;
    size_t i = heap.sz - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap.arr[p].val <= heap.arr[i].val) break;
        Node tmp = heap.arr[p];
        heap.arr[p] = heap.arr[i];
        heap.arr[i] = tmp;
        i = p;
    }
}

static Node heap_pop(void) {
    Node top = heap.arr[0];
    heap.arr[0] = heap.arr[--heap.sz];
    size_t i = 0;
    while (true) {
        size_t l = i * 2 + 1, r = l + 1, s = i;
        if (l < heap.sz && heap.arr[l].val < heap.arr[s].val) s = l;
        if (r < heap.sz && heap.arr[r].val < heap.arr[s].val) s = r;
        if (s == i) break;
        Node tmp = heap.arr[i];
        heap.arr[i] = heap.arr[s];
        heap.arr[s] = tmp;
        i = s;
    }
    return top;
}

/* ---------- Cache growth ---------- */
static void cache_append(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t newcap = (cache_cap ? cache_cap * 2 : 16);
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

/* ---------- Ensure cache has at least `need` primes ---------- */
static void ensure_cache(idx_t need) {
    if (need <= cache_len) return;

    if (!initialized) {
        const idx_t MAX_INIT_N = 5000;  /* enough for n≤10000 */
        heap_init((size_t)MAX_INIT_N + 1);
        for (idx_t n = 0; n <= MAX_INIT_N; ++n) {
            seq_t base = (seq_t)(4 * n + 1);
            base = base * base;   /* (4n+1)^2 */
            Node nd = { base, n, 0 };
            heap_push(&nd);
        }
        initialized = true;
    }

    while (cache_len < need) {
        Node nd = heap_pop();
        if (nd.val > 1 && is_prime_big(nd.val))
            cache_append(nd.val);

        /* push the next candidate for the same n */
        seq_t inc = (seq_t)16 * ((seq_t)(2 * nd.m + 1)); /* (4(m+1))^2 - (4m)^2 */
        Node nxt = { nd.val + inc, nd.n, nd.m + 1 };
        heap_push(&nxt);
    }
}

/* ---------- The required function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n - 1];
}
