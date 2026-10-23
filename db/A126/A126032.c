/*
 * A126032  Numbers of the form b^m/2 for even b and odd m > 2.
 * https://oeis.org/A126032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126032: a(n) = the n-th distinct integer of the form
   b^m/2 where b is even and m is odd greater than 2. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

/* ------------------------------------------------------------------ */
/* ----------  Heap element ---------------------------------------- */
/* ------------------------------------------------------------------ */
typedef struct {
    seq_t val;        /* current value b^m / 2  */
    uint64_t base;    /* even base b            */
    seq_t square;     /* base^2 as seq_t        */
} Node;

/* ------------------------------------------------------------------ */
/* ----------  Min‑heap utilities ---------------------------------- */
/* ------------------------------------------------------------------ */
static Node *heap = NULL;      /* array of heap nodes          */
static size_t heap_size = 0;   /* number of nodes in the heap  */
static size_t heap_cap   = 0;  /* capacity of the array        */

static inline void heap_swap(Node *a, Node *b) {
    Node tmp = *a; *a = *b; *b = tmp;
}

static void heap_sift_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        heap_swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

static void heap_sift_down(size_t i) {
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap_size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

static void heap_push(Node node) {
    if (heap_size + 1 > heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 256;
        heap = (Node*)realloc(heap, heap_cap * sizeof(Node));
    }
    heap[heap_size++] = node;
    heap_sift_up(heap_size - 1);
}

static Node heap_pop(void) {
    Node min = heap[0];
    heap[0] = heap[--heap_size];
    heap_sift_down(0);
    return min;
}

/* ------------------------------------------------------------------ */
/* ----------  Memoised sequence cache ----------------------------- */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;     /* array holding the generated terms */
static size_t cache_len = 0;    /* number of stored terms            */
static size_t cache_cap = 0;    /* capacity of the array             */

static idx_t next_base = 4;     /* next even base to consider          */
static bool initialised = false;

/* ------------------------------------------------------------------ */
/* ----------  Main sequence routine ------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* --- initialise data structures on first call ----------------- */
    if (!initialised) {
        cache_cap = 256;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));

        heap_cap = 256;
        heap = (Node*)calloc(heap_cap, sizeof(Node));

        /* first base is 2: 2^3/2 = 4 */
        seq_t init = ((seq_t)2 * (seq_t)2 * (seq_t)2) >> 1;
        Node first = { init, 2, (seq_t)2 * (seq_t)2 };
        heap_push(first);

        next_base = 4;          /* start with the next even base */
        initialised = true;
    }

    /* --- generate terms until we have at least n ---------- */
    while (cache_len < n) {
        /* add new bases whose first term is not larger than the
           current smallest value in the heap; this guarantees the
           next smallest value is always available. */
        while (next_base > 0) {
            seq_t val = ((seq_t)next_base * (seq_t)next_base *
                         (seq_t)next_base) >> 1;   /* b^3 / 2 */
            if (heap_size == 0 || val <= heap[0].val) {
                seq_t sq = (seq_t)next_base * (seq_t)next_base;
                Node node = { val, next_base, sq };
                heap_push(node);
                next_base += 2;
            } else {
                break;
            }
        }

        /* extract the smallest current term */
        Node cur = heap_pop();

        /* store it in the cache if it differs from the last value
           (this removes duplicates such as 256 from base 2 and 8). */
        if (cache_len == 0 || cur.val != cache[cache_len - 1]) {
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 256;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur.val;
        }

        /* compute the next term from the same base (m increases by 2) */
        bool push_next = true;
#if !defined(__SIZEOF_INT128__) && defined(ULLONG_MAX)
        if (sizeof(seq_t) <= 8) { /* 64‑bit type */
            if (cur.val > 0 && cur.square > 0 &&
                cur.val > (ULLONG_MAX / cur.square))
                push_next = false;        /* would overflow */
        }
#endif
        if (push_next) {
            Node next_node = { cur.val * cur.square, cur.base, cur.square };
            heap_push(next_node);
        }
    }

    return cache[n - 1];   /* indices start at 1 */
}
