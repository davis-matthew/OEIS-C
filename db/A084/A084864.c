/*
 * A084864  Numbers that can be written in the form 2*u^2 + 3*v^2, u*v>0.
 * https://oeis.org/A084864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084864: numbers that can be written as 2*u^2 + 3*v^2 for u,v>0 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */

#if defined(__cplusplus)
inline bool seq_less(seq_t a, seq_t b) { return a < b; }
#else
#define seq_less(a,b) ((a) < (b))
#endif

/* Node for the priority queue */
struct Node {
    seq_t val;
    idx_t u;
    idx_t v;
};

/* Static storage for the generated sequence and the priority queue */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static struct Node *heap       = NULL;
static size_t heap_size        = 0;
static size_t heap_cap         = 0;
static idx_t  next_u           = 1;  /* next u value to be pushed into the heap */

/* Heap auxiliary functions */
static void heap_sift_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (seq_less(heap[parent].val, heap[i].val))
            break;
        struct Node tmp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = tmp;
        i = parent;
    }
}

static void heap_sift_down(size_t i) {
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && seq_less(heap[l].val, heap[smallest].val))
            smallest = l;
        if (r < heap_size && seq_less(heap[r].val, heap[smallest].val))
            smallest = r;
        if (smallest == i)
            break;
        struct Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void heap_push(seq_t val, idx_t u, idx_t v) {
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 64;
        heap = (struct Node *)realloc(heap, new_cap * sizeof(struct Node));
        heap_cap = new_cap;
    }
    heap[heap_size].val = val;
    heap[heap_size].u   = u;
    heap[heap_size].v   = v;
    heap_sift_up(heap_size);
    heap_size++;
}

static struct Node heap_pop_min(void) {
    struct Node top = heap[0];
    heap_size--;
    if (heap_size) {
        heap[0] = heap[heap_size];
        heap_sift_down(0);
    }
    return top;
}

/* Generate sequence up to 'needed' terms */
static void generate_until(size_t needed) {
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    while (cache_len < needed) {
        /* Ensure heap has at least one element */
        if (heap_size == 0) {
            seq_t first_val = (seq_t)2 * (seq_t)next_u * (seq_t)next_u + (seq_t)3;
            heap_push(first_val, next_u, 1);
            next_u++;
        }

        /* Insert all new u's whose minimal value does not exceed the current top */
        while (next_u && ( (seq_t)2 * (seq_t)next_u * (seq_t)next_u + (seq_t)3 ) <= heap[0].val) {
            seq_t val = (seq_t)2 * (seq_t)next_u * (seq_t)next_u + (seq_t)3;
            heap_push(val, next_u, 1);
            next_u++;
        }

        /* Extract minimum */
        struct Node cur = heap_pop_min();

        /* Deduplicate */
        if (cache_len == 0 || cur.val != cache[cache_len - 1]) {
            cache[cache_len++] = cur.val;
        }

        /* Push next candidate for the same u */
        idx_t v_next = cur.v + 1;
        seq_t val_next = (seq_t)2 * (seq_t)cur.u * (seq_t)cur.u + (seq_t)3 * (seq_t)v_next * (seq_t)v_next;
        heap_push(val_next, cur.u, v_next);
    }
}

/* Public interface: return the n-th term (1‑based) */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* undefined, but safe */
    generate_until((size_t)n);
    return cache[(size_t)n - 1];
}
