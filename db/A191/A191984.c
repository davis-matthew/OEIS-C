/*
 * A191984  Monotonically ordered sums 2^(i-1)+3*2^(j-1), for i>=1, j>=1.
 * https://oeis.org/A191984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest distinct value of 2^(i-1)+3·2^(j-1)
 *            for i,j ≥ 1
 * Range:     u8 n <= 37, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   sum_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sum_val.h"

/* ---------- visited set for (i,j) pairs ----------------------------------- */
static uint64_t *visited_keys = NULL;
static size_t visited_cap = 0;
static size_t visited_cnt = 0;

static void visited_init(void) {
    visited_cap = 1 << 16;                     /* 65536 */
    visited_keys = (uint64_t*)calloc(visited_cap, sizeof(uint64_t));
    visited_cnt = 0;
}

static void visited_rehash(void) {
    size_t new_cap = visited_cap << 1;
    uint64_t *new_keys = (uint64_t*)calloc(new_cap, sizeof(uint64_t));
    for (size_t i = 0; i < visited_cap; ++i) {
        uint64_t k = visited_keys[i];
        if (k) {
            size_t idx = k & (new_cap - 1);
            while (new_keys[idx]) idx = (idx + 1) & (new_cap - 1);
            new_keys[idx] = k;
        }
    }
    free(visited_keys);
    visited_keys = new_keys;
    visited_cap = new_cap;
}

static bool visited_insert(uint64_t key) {
    if (visited_cnt * 2 >= visited_cap) visited_rehash();
    size_t idx = key & (visited_cap - 1);
    while (visited_keys[idx]) {
        if (visited_keys[idx] == key) return false;  /* already seen */
        idx = (idx + 1) & (visited_cap - 1);
    }
    visited_keys[idx] = key;
    ++visited_cnt;
    return true;   /* inserted */
}

/* ---------- min‑heap for nodes ------------------------------------------- */
struct Node { seq_t val; idx_t i, j; };

static struct Node *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_sz  = 0;

static void heap_init(void) {
    heap_cap = 64;
    heap = (struct Node*)malloc(heap_cap * sizeof(struct Node));
    heap_sz = 0;
}

static void heap_free(void) { free(heap); }

static void heap_swap(struct Node *a, struct Node *b) {
    struct Node tmp = *a; *a = *b; *b = tmp;
}

static void heap_push(const struct Node *node) {
    if (heap_sz >= heap_cap) {
        heap_cap <<= 1;
        heap = (struct Node*)realloc(heap, heap_cap * sizeof(struct Node));
    }
    size_t k = heap_sz++;
    heap[k] = *node;
    while (k > 0) {
        size_t p = (k - 1) >> 1;
        if (heap[p].val <= heap[k].val) break;
        heap_swap(&heap[p], &heap[k]);
        k = p;
    }
}

static struct Node heap_pop(void) {
    struct Node root = heap[0];
    heap[0] = heap[--heap_sz];
    size_t k = 0;
    while (1) {
        size_t l = (k << 1) + 1;
        size_t r = l + 1;
        size_t best = k;
        if (l < heap_sz && heap[l].val < heap[best].val) best = l;
        if (r < heap_sz && heap[r].val < heap[best].val) best = r;
        if (best == k) break;
        heap_swap(&heap[k], &heap[best]);
        k = best;
    }
    return root;
}

/* ---------- main function ------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* below offset, irrelevant */

    visited_init();
    heap_init();

    /* start from i=j=1 */
    struct Node start;
    start.val = sum_val(1, 1);
    start.i   = 1;
    start.j   = 1;
    heap_push(&start);
    visited_insert(((uint64_t)1 << 32) | (uint64_t)1);

    seq_t last   = 0;     /* sentinel */
    seq_t result = start.val;
    idx_t count  = 0;

    while (heap_sz > 0 && count < n) {
        struct Node cur = heap_pop();

        if (cur.val != last) {
            last = cur.val;
            ++count;
            if (count == n) {
                result = cur.val;
                break;
            }
        }

        /* neighbors */
        idx_t ii = cur.i + 1, jj = cur.j;
        uint64_t key = ((uint64_t)ii << 32) | (uint64_t)jj;
        if (visited_insert(key)) {
            struct Node nd = { sum_val(ii, jj), ii, jj };
            heap_push(&nd);
        }

        ii = cur.i;  jj = cur.j + 1;
        key = ((uint64_t)ii << 32) | (uint64_t)jj;
        if (visited_insert(key)) {
            struct Node nd = { sum_val(ii, jj), ii, jj };
            heap_push(&nd);
        }
    }

    heap_free();
    free(visited_keys);
    return result;
}
