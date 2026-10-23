/*
 * A184399  Ranks of (odd i)+(odd j)*r, when all i+j*r are ranked; r=sqrt(2),
 *          i>=0, j>=0.
 * https://oeis.org/A184399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of the n‑th pair (i,j) with i,j odd in the ordering
 *            * of all pairs (i,j) with i,j ≥ 0 by the value i + j·√2. * The
 *            code generates all pairs in increasing order using a min‑heap *
 *            and a visited table to avoid duplicates. * It returns the rank
 *            (1‑based) at which the nth odd‑odd pair app...
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct HeapNode {
    long double val;
    idx_t i;
    idx_t j;
};

#define HEAP_CAP 200000
#define VISIT_CAP (1 << 18)  /* 262144 entries */

static inline void heap_swap(struct HeapNode *a, struct HeapNode *b)
{
    struct HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(struct HeapNode *heap, size_t *size, struct HeapNode node)
{
    size_t pos = *size;
    heap[pos] = node;
    (*size)++;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].val <= heap[pos].val)
            break;
        heap_swap(&heap[parent], &heap[pos]);
        pos = parent;
    }
}

static struct HeapNode heap_pop(struct HeapNode *heap, size_t *size)
{
    struct HeapNode root = heap[0];
    (*size)--;
    heap[0] = heap[*size];
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < *size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < *size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == pos)
            break;
        heap_swap(&heap[pos], &heap[smallest]);
        pos = smallest;
    }
    return root;
}

/* Simple open addressing hash set for visited (i,j) pairs */
static uint64_t visit_keys[VISIT_CAP];
static uint8_t visit_used[VISIT_CAP];
static const uint64_t VISIT_MASK = VISIT_CAP - 1;

static void visit_clear(void)
{
    memset(visit_used, 0, sizeof visit_used);
}

static uint64_t hash_key(uint64_t key)
{
    key ^= key >> 33;
    key *= 0xff51afd7ed558ccdULL;
    key ^= key >> 33;
    key *= 0xc4ceb9fe1a85ec53ULL;
    key ^= key >> 33;
    return key & VISIT_MASK;
}

static int visit_contains(uint64_t key)
{
    uint64_t h = hash_key(key);
    while (visit_used[h]) {
        if (visit_keys[h] == key)
            return 1;
        h = (h + 1) & VISIT_MASK;
    }
    return 0;
}

static void visit_insert(uint64_t key)
{
    uint64_t h = hash_key(key);
    while (visit_used[h]) {
        if (visit_keys[h] == key)
            return;
        h = (h + 1) & VISIT_MASK;
    }
    visit_used[h] = 1;
    visit_keys[h] = key;
}

seq_t a(idx_t n)
{
    /* Ensure sqrt(2) is computed once */
    static int initialized = 0;
    static long double sqrt2;
    if (!initialized) {
        sqrt2 = sqrtl(2.0L);
        initialized = 1;
    }

    /* Reset visited table */
    visit_clear();

    static struct HeapNode heap[HEAP_CAP];
    size_t heap_size = 0;

    /* Push starting node (0,0) */
    struct HeapNode start = {0.0L, 0, 0};
    heap_push(heap, &heap_size, start);
    visit_insert(0ULL);  /* key = 0 for (0,0) */

    idx_t oddCnt = 0;
    idx_t rank = 0;

    while (oddCnt < n) {
        struct HeapNode cur = heap_pop(heap, &heap_size);
        rank++;

        if ((cur.i & 1U) && (cur.j & 1U)) {
            oddCnt++;
            if (oddCnt == n)
                return (seq_t)rank;
        }

        /* child (i+1, j) */
        idx_t ci = cur.i + 1;
        idx_t cj = cur.j;
        uint64_t key1 = (((uint64_t)ci) << 32) | cj;
        if (!visit_contains(key1)) {
            visit_insert(key1);
            struct HeapNode child;
            child.i = ci;
            child.j = cj;
            child.val = cur.val + 1.0L;
            heap_push(heap, &heap_size, child);
        }

        /* child (i, j+1) */
        ci = cur.i;
        cj = cur.j + 1;
        uint64_t key2 = (((uint64_t)ci) << 32) | cj;
        if (!visit_contains(key2)) {
            visit_insert(key2);
            struct HeapNode child;
            child.i = ci;
            child.j = cj;
            child.val = cur.val + sqrt2;
            heap_push(heap, &heap_size, child);
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}
