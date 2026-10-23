/*
 * A184397  Ranks of (odd i)+j*r, when all i+j*r are ranked; r=sqrt(2), i>=0,
 *          j>=0. Complement of A184398.
 * https://oeis.org/A184397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   sign_of_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate points i + j*sqrt(2) in increasing order using a
   min‑heap; collect the ranks (1‑based) where i is odd.  The function
   a(n) returns the n‑th such rank (OEIS offset 1). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sign_of_sum.h"

/* Node representing a lattice point (i,j) */
typedef struct { idx_t i; idx_t j; } Node;

static int cmp_node(const Node *a, const Node *b)
{
    int64_t du = (int64_t)a->i - (int64_t)b->i;
    int64_t dv = (int64_t)a->j - (int64_t)b->j;
    return sign_of_sum(du, dv);   /* negative if a<b */
}

/* --------------------- heap operations --------------------- */
static void heap_up(Node *heap, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (cmp_node(&heap[idx], &heap[parent]) < 0) {
            Node tmp = heap[idx];
            heap[idx] = heap[parent];
            heap[parent] = tmp;
            idx = parent;
        } else {
            break;
        }
    }
}

static void heap_down(Node *heap, size_t sz, size_t idx)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < sz && cmp_node(&heap[left], &heap[smallest]) < 0)
            smallest = left;
        if (right < sz && cmp_node(&heap[right], &heap[smallest]) < 0)
            smallest = right;
        if (smallest == idx)
            break;
        Node tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

static void heap_push(Node **heap_ptr, size_t *sz, size_t *cap, Node v)
{
    if (*sz >= *cap) {
        *cap = (*cap == 0) ? 4 : (*cap << 1);
        Node *new_heap = (Node*)realloc(*heap_ptr, *cap * sizeof(Node));
        if (!new_heap) abort();
        *heap_ptr = new_heap;
    }
    (*heap_ptr)[*sz] = v;
    heap_up(*heap_ptr, *sz);
    (*sz)++;
}

static Node heap_pop(Node *heap, size_t *sz)
{
    Node top = heap[0];
    (*sz)--;
    if (*sz > 0) {
        heap[0] = heap[*sz];
        heap_down(heap, *sz, 0);
    }
    return top;
}

/* --------------------- hash set for visited pairs --------------------- */
typedef struct {
    size_t cap;
    size_t size;
    uint64_t *keys;      /* (uint64_t)-1 marks empty slot */
} HashSet;

static void hs_init(HashSet *hs, size_t min_cap)
{
    size_t cap = 1;
    while (cap < min_cap) cap <<= 1;
    hs->cap = cap;
    hs->size = 0;
    hs->keys = (uint64_t*)malloc(cap * sizeof(uint64_t));
    for (size_t i = 0; i < cap; ++i)
        hs->keys[i] = (uint64_t)-1;
}

static void hs_rehash(HashSet *hs)
{
    size_t old_cap = hs->cap;
    uint64_t *old_keys = hs->keys;
    hs->cap <<= 1;
    hs->keys = (uint64_t*)malloc(hs->cap * sizeof(uint64_t));
    for (size_t i = 0; i < hs->cap; ++i)
        hs->keys[i] = (uint64_t)-1;
    hs->size = 0;
    for (size_t i = 0; i < old_cap; ++i) {
        uint64_t k = old_keys[i];
        if (k != (uint64_t)-1) {
            size_t idx = (k ^ (k >> 33) ^ (k >> 17)) & (hs->cap - 1);
            while (hs->keys[idx] != (uint64_t)-1)
                idx = (idx + 1) & (hs->cap - 1);
            hs->keys[idx] = k;
            hs->size++;
        }
    }
    free(old_keys);
}

static bool hs_insert(HashSet *hs, uint64_t key)
{
    size_t idx = (key ^ (key >> 33) ^ (key >> 17)) & (hs->cap - 1);
    while (hs->keys[idx] != (uint64_t)-1) {
        if (hs->keys[idx] == key)
            return false;           /* already present */
        idx = (idx + 1) & (hs->cap - 1);
    }
    hs->keys[idx] = key;
    hs->size++;
    if (hs->size * 5 >= hs->cap * 3)   /* load factor > 0.6 */
        hs_rehash(hs);
    return true;
}

/* --------------------- dynamic array for odd ranks --------------------- */
static seq_t *odd_array = NULL;
static size_t odd_cap = 0, odd_sz = 0;

static void odd_push(seq_t val)
{
    if (odd_sz >= odd_cap) {
        odd_cap = odd_cap ? odd_cap << 1 : 16;
        odd_array = (seq_t*)realloc(odd_array, odd_cap * sizeof(seq_t));
    }
    odd_array[odd_sz++] = val;
}

/* --------------------- main function --------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* never used, offset starts at 1 */

    /* static state across calls */
    static bool initialized = false;
    static Node *heap = NULL;
    static size_t heap_sz = 0, heap_cap = 0;
    static HashSet visited;
    static idx_t current_rank = 0;

    if (!initialized) {
        /* initializations */
        hs_init(&visited, 1 << 14);          /* initial capacity */
        hs_insert(&visited, 0);              /* key for (0,0) */
        heap_push(&heap, &heap_sz, &heap_cap, (Node){0,0});
        initialized = true;
    }

    /* generate until we have enough odd ranks */
    while (odd_sz < n) {
        if (heap_sz == 0) return 0;  /* should not happen */
        Node cur = heap_pop(heap, &heap_sz);
        current_rank++;

        if (cur.i & 1u) {           /* i is odd */
            seq_t rnk = (seq_t)current_rank;
            odd_push(rnk);
        }

        /* push (i+1,j) and (i,j+1) if not seen */
        uint64_t key1 = (((uint64_t)(cur.i + 1)) << 32) | cur.j;
        if (hs_insert(&visited, key1))
            heap_push(&heap, &heap_sz, &heap_cap, (Node){cur.i + 1, cur.j});

        uint64_t key2 = ((uint64_t)cur.i << 32) | (cur.j + 1);
        if (hs_insert(&visited, key2))
            heap_push(&heap, &heap_sz, &heap_cap, (Node){cur.i, cur.j + 1});
    }

    return odd_array[n - 1];
}
