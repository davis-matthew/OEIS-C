/*
 * A192045  Ordered sums 3*f+5*g, where f and g are Lucas numbers (A000032
 *          beginning at 1).
 * https://oeis.org/A192045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*L_i + 5*L_j for all i,j>=1, sorted increasingly with
 *            Lucas numbers L_1=1, L_2=3, L_{k}=L_{k-1}+L_{k-2}
 * Range:     u8 n <= 46, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>


/*--------------------------------------------------------------------*/
/*  Lucas numbers caching                                           */
/*--------------------------------------------------------------------*/
static seq_t *lucas = NULL;      /* Lucas numbers array              */
static size_t lucas_len = 0;      /* number of computed Lucas numbers */
static size_t lucas_cap = 0;      /* allocated capacity */

static void ensure_lucas(idx_t idx) {
    size_t needed = (size_t)idx + 1;        /* 0‑based index */
    if (lucas_len >= needed) return;

    if (lucas == NULL) {
        lucas_cap = 8;
        lucas = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        lucas[0] = (seq_t)1;               /* L_1 */
        lucas[1] = (seq_t)3;               /* L_2 */
        lucas_len = 2;
    }

    if (lucas_cap < needed)
        lucas_cap = needed * 2;
    lucas = (seq_t *)realloc(lucas, lucas_cap * sizeof(seq_t));

    while (lucas_len < needed) {
        lucas[lucas_len] = lucas[lucas_len - 1] + lucas[lucas_len - 2];
        lucas_len++;
    }
}

/*--------------------------------------------------------------------*/
/*  Simple open addressing hash set for visited (i,j) pairs           */
/*--------------------------------------------------------------------*/
#define VIS_EMPTY 0xFFFFFFFFFFFFFFFFULL

typedef struct {
    size_t cap;          /* capacity (power of two) */
    size_t mask;         /* cap-1 */
    size_t size;         /* number of keys stored */
    uint64_t *keys;      /* array of keys */
} VisHash;

static VisHash visited = {0, 0, 0, NULL};

static void visited_init(void) {
    visited.cap = 16;
    visited.mask = visited.cap - 1;
    visited.size = 0;
    visited.keys = (uint64_t *)malloc(visited.cap * sizeof(uint64_t));
    for (size_t i = 0; i < visited.cap; i++)
        visited.keys[i] = VIS_EMPTY;
}

static void visited_resize(void) {
    size_t new_cap = visited.cap * 2;
    size_t new_mask = new_cap - 1;
    uint64_t *new_keys = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    for (size_t i = 0; i < new_cap; i++)
        new_keys[i] = VIS_EMPTY;

    for (size_t i = 0; i < visited.cap; i++) {
        uint64_t k = visited.keys[i];
        if (k != VIS_EMPTY) {
            size_t h = ((k ^ (k >> 33) ^ (k >> 17)) & new_mask);
            while (new_keys[h] != VIS_EMPTY)
                h = (h + 1) & new_mask;
            new_keys[h] = k;
        }
    }
    free(visited.keys);
    visited.keys = new_keys;
    visited.cap = new_cap;
    visited.mask = new_mask;
}

/* Insert key into visited hash set.
   Return 1 if key was newly inserted, 0 if it was already present.   */
static int visited_insert(uint64_t key) {
    if (visited.cap == 0) visited_init();

    /* Resize if load factor > 0.7 */
    if (visited.size + 1 > visited.cap * 7 / 10)
        visited_resize();

    size_t h = ((key ^ (key >> 33) ^ (key >> 17)) & visited.mask);
    while (visited.keys[h] != VIS_EMPTY && visited.keys[h] != key)
        h = (h + 1) & visited.mask;

    if (visited.keys[h] == key) return 0; /* already present */

    visited.keys[h] = key;
    visited.size++;
    return 1;
}

/*--------------------------------------------------------------------*/
/*  Min‑heap for nodes (sum,i,j)                                     */
/*--------------------------------------------------------------------*/
typedef struct {
    seq_t sum;
    idx_t i;
    idx_t j;
} HeapNode;

static HeapNode *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap = 0;

static void heap_push(idx_t i, idx_t j) {
    uint64_t key = ((uint64_t)i << 32) | (uint64_t)j;
    if (!visited_insert(key)) return;           /* already processed */

    ensure_lucas(i);
    ensure_lucas(j);

    HeapNode node;
    node.i = i;
    node.j = j;
    node.sum = lucas[i] * (seq_t)3 + lucas[j] * (seq_t)5;

    if (heap_size >= heap_cap) {
        heap_cap = heap_cap == 0 ? 16 : heap_cap * 2;
        heap = (HeapNode *)realloc(heap, heap_cap * sizeof(HeapNode));
    }

    size_t pos = heap_size++;
    heap[pos] = node;
    /* bubble up */
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].sum <= heap[pos].sum) break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[pos];
        heap[pos] = tmp;
        pos = parent;
    }
}

static HeapNode heap_pop_min(void) {
    HeapNode ret = heap[0];
    heap[0] = heap[--heap_size];
    /* bubble down */
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;
        if (left < heap_size && heap[left].sum < heap[smallest].sum)
            smallest = left;
        if (right < heap_size && heap[right].sum < heap[smallest].sum)
            smallest = right;
        if (smallest == pos) break;
        HeapNode tmp = heap[pos];
        heap[pos] = heap[smallest];
        heap[smallest] = tmp;
        pos = smallest;
    }
    return ret;
}

/*--------------------------------------------------------------------*/
/*  Generated sequence array                                         */
/*--------------------------------------------------------------------*/
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void append_seq(seq_t val) {
    if (seq_len >= seq_cap) {
        seq_cap = seq_cap == 0 ? 64 : seq_cap * 2;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
    seq_arr[seq_len++] = val;
}

/*--------------------------------------------------------------------*/
/*  Sequence function                                                */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n<1 */

    /* Generate terms until we have at least n terms. */
    while (seq_len < n) {
        /* Seed initial node if heap is empty. */
        if (heap_size == 0)
            heap_push(0, 0);

        HeapNode node = heap_pop_min();

        /* Skip duplicate sums. */
        if (seq_len > 0 && node.sum == seq_arr[seq_len - 1]) {
            /* Push successors of this node. */
            heap_push(node.i + 1, node.j);
            heap_push(node.i, node.j + 1);
            continue;
        }

        append_seq(node.sum);

        /* Push successors of this node. */
        heap_push(node.i + 1, node.j);
        heap_push(node.i, node.j + 1);
    }

    return seq_arr[n - 1];
}
