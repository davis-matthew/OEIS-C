/*
 * A025656  Exponent of 6 (value of j) in n-th number of form 2^i*6^j.
 * https://oeis.org/A025656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025656: a(n) is the exponent of 6 in the nth smallest
   integer of the form 2^i * 6^j (offset 1).  The algorithm generates
   all numbers 2^i*6^j in increasing order with a min‑heap and records
   the exponent j. */

/* Required headers for memory allocation, integer types and bool. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------- */
/*  Hash table for visited (k,j) pairs.  Key = (k << 32) | j.  */
#define HASH_CAP (1U << 15)                 /* 32768 slots */
static uint64_t visited[HASH_CAP] = {0};

/* Check if a pair has already been inserted. */
static inline bool visited_contains(uint64_t key)
{
    uint64_t val = key + 1;                /* shift to avoid 0 as empty */
    uint64_t h   = val & (HASH_CAP - 1);
    while (visited[h] != 0) {
        if (visited[h] == val) return true;
        h = (h + 1) & (HASH_CAP - 1);
    }
    return false;
}

/* Insert a pair into the hash table. */
static inline void visited_insert(uint64_t key)
{
    uint64_t val = key + 1;
    uint64_t h   = val & (HASH_CAP - 1);
    while (visited[h] != 0) h = (h + 1) & (HASH_CAP - 1);
    visited[h] = val;
}

/* ---------------------------------------------------------------------- */
/*  Min‑heap of nodes (k = exponent of 2, j = exponent of 6, key = log₂(value)). */
typedef struct {
    int  k;      /* exponent of 2 in the representation 2^k * 3^j */
    int  j;      /* exponent of 6 */
    double key;  /* key for ordering; k + j*log₂(3) */
} Node;

/* Simple binary min‑heap. */
typedef struct {
    size_t  size;
    size_t  cap;
    Node *data;          /* casted pointer to Node */
} Heap;
static Heap heap = {0, 0, NULL};

static void heap_init(void)
{
    if (heap.cap == 0) {
        heap.cap  = 16;
        heap.data = (Node *)malloc(heap.cap * sizeof(Node));
    }
}

/* Swap two heap nodes. */
static inline void heap_swap(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}

/* Push a node onto the heap. */
static void heap_push(Node n)
{
    heap_init();
    if (heap.size == heap.cap) {
        heap.cap <<= 1;
        heap.data = (Node *)realloc(heap.data, heap.cap * sizeof(Node));
    }
    size_t i = heap.size++;
    heap.data[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap.data[p].key <= heap.data[i].key) break;
        heap_swap(&heap.data[p], &heap.data[i]);
        i = p;
    }
}

/* Pop minimum node from the heap. */
static Node heap_pop(void)
{
    Node top = heap.data[0];
    heap.data[0] = heap.data[--heap.size];
    size_t i = 0;
    while (true) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap.size &&
            (heap.data[l].key < heap.data[smallest].key ||
             (heap.data[l].key == heap.data[smallest].key &&
              heap.data[l].j < heap.data[smallest].j))) smallest = l;
        if (r < heap.size &&
            (heap.data[r].key < heap.data[smallest].key ||
             (heap.data[r].key == heap.data[smallest].key &&
              heap.data[r].j < heap.data[smallest].j))) smallest = r;
        if (smallest == i) break;
        heap_swap(&heap.data[i], &heap.data[smallest]);
        i = smallest;
    }
    return top;
}

/* ---------------------------------------------------------------------- */
/*  Cache of sequence terms.  */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_ensure(size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    while (new_cap < n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* sequence defined for n >= 1 */

    /* Grow the term cache to accommodate n terms. */
    if ((size_t)n > cache_len) {
        cache_ensure(n);

        /* Initialise heap and visited set once. */
        if (heap.size == 0) {
            Node root = {0, 0, 0.0};
            heap_push(root);
            visited_insert(0);
        }

        const double LOG2_3 = 1.584962500721156;   /* log₂(3) */

        while (cache_len < (size_t)n) {
            Node cur = heap_pop();
            cache[cache_len++] = (seq_t)cur.j;     /* store exponent of 6 */

            /* Child 1: multiply by 2 → (k+1, j). */
            int k1 = cur.k + 1, j1 = cur.j;
            uint64_t key1 = ((uint64_t)k1 << 32) | (uint64_t)j1;
            if (!visited_contains(key1)) {
                visited_insert(key1);
                Node nb = {k1, j1, k1 + j1 * LOG2_3};
                heap_push(nb);
            }

            /* Child 2: multiply by 6 → (k+1, j+1). */
            int k2 = cur.k + 1, j2 = cur.j + 1;
            uint64_t key2 = ((uint64_t)k2 << 32) | (uint64_t)j2;
            if (!visited_contains(key2)) {
                visited_insert(key2);
                Node nb = {k2, j2, k2 + j2 * LOG2_3};
                heap_push(nb);
            }
        }
    }
    return cache[n - 1];
}
