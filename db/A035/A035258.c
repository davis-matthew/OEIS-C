/*
 * A035258  Positive integers of the form 2x^2+xy-2y^2 (discriminant 17).
 * https://oeis.org/A035258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Positive integers of the form 2x^2 + x*y - 2y^2 (discriminant
 *            17)
 * Range:     u8 none, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth positive value representable by that quadratic form, sorted
   in ascending order.  The function is memoised and works for any idx_t n >= 1. */
/* --------------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Node in the min‑heap: value, current coordinates (x,y) and which side
   of the parabola (0 = left, 1 = right).  */
typedef struct {
    seq_t val;      /* current positive value */
    int64_t x, y;   /* current integer coordinates */
    int     side;   /* 0 = moving left, 1 = moving right */
} Node;

/* --------------------------------------------------------------------------- */
/* Heaps and cache */

#define RADIUS          2000          /* covers far more than needed for n<=10000 */
#define HEAP_CAP        (2 * (2 * RADIUS + 1) + 5)   /* space for all initial nodes */
#define MAX_TERMS       200000        /* number of values pre‑cached initially */

static Node      heap[HEAP_CAP];
static size_t    heap_sz = 0;
static bool      heap_inited = false;

/* ----------------------------------------------------------------------------
   Helper functions for the binary min‑heap.
   --------------------------------------------------------------------------- */
static inline void heap_swap(Node *a, Node *b) {
    Node tmp = *a; *a = *b; *b = tmp;
}

static void heap_push(const Node *nd) {
    heap[heap_sz++] = *nd;
    size_t i = heap_sz - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

static Node heap_pop(void) {
    Node ret = heap[0];
    heap[0] = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t child = (r < heap_sz && heap[r].val < heap[l].val) ? r : l;
        if (heap[i].val <= heap[child].val) break;
        heap_swap(&heap[i], &heap[child]);
        i = child;
    }
    return ret;
}

/* ----------------------------------------------------------------------------
   Initialise the heap with the first positive value on both sides
   of the parabola for every integer y in [-RADIUS, RADIUS].
   --------------------------------------------------------------------------- */
static void init_heap(void) {
    if (heap_inited) return;
    double d_root_factor = sqrt((double)17.0) - 1.0;          /* (sqrt(17)-1)/4 factor */
    double d_left_factor = -(1.0 + sqrt((double)17.0));      /* -(1+sqrt(17))/4 factor */

    for (int64_t y = -(int64_t)RADIUS; y <= (int64_t)RADIUS; ++y) {
        /* ---- right side (x > root2) ---- */
        double r2 = ((double)y) * d_root_factor / 4.0;
        int64_t x = (int64_t)ceil(r2);
        while (1) {
            long long v = 2LL*x*x + (long long)x*y - 2LL*y*y;
            if (v > 0) {
                Node n = { (seq_t)v, x, y, 1 };
                heap_push(&n);
                break;
            }
            ++x;
        }

        /* ---- left side (x < root1) ---- */
        double r1 = ((double)y) * d_left_factor / 4.0;
        int64_t x2 = (int64_t)floor(r1);
        while (1) {
            long long v = 2LL*x2*x2 + (long long)x2*y - 2LL*y*y;
            if (v > 0) {
                Node n = { (seq_t)v, x2, y, 0 };
                heap_push(&n);
                break;
            }
            --x2;
        }
    }

    heap_inited = true;
}

/* ----------------------------------------------------------------------------
   Cache of distinct sequence values already produced.
   --------------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_sz = 0;

/* ----------------------------------------------------------------------------
   Generate the first MAX_TERMS distinct values and store them in cache.
   --------------------------------------------------------------------------- */
static void precompute_cache(void) {
    if (cache_sz > 0) return;   /* already done */

    init_heap();

    cache = (seq_t*)malloc(MAX_TERMS * sizeof(seq_t));
    if (!cache) return;        /* allocation failure – give up */

    seq_t last_val = 0;
    bool first = true;

    while (cache_sz < MAX_TERMS) {
        Node cur = heap_pop();

        /* Compute next candidate for the same side and push it back */
        int64_t nx = (cur.side == 1) ? cur.x + 1 : cur.x - 1;
        long long next_v = 2LL*nx*nx + (long long)nx*cur.y - 2LL*cur.y*cur.y;
        if (next_v > 0) {
            Node nxt = { (seq_t)next_v, nx, cur.y, cur.side };
            heap_push(&nxt);
        }

        seq_t val = cur.val;
        if (first || val != last_val) {
            cache[cache_sz++] = val;
            last_val = val;
            first = false;
        }
        /* duplicates are simply skipped */
    }
}

/* ----------------------------------------------------------------------------
   The main sequence function.
   --------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety guard, though n>=1 per spec */

    precompute_cache();

    /* If the requested index is within the precomputed cache, return immediately */
    if (n <= cache_sz) return cache[n - 1];

    /* Otherwise generate further distinct values until we reach the required index. */
    seq_t last_val = cache[cache_sz - 1];
    while (cache_sz < n) {
        Node cur = heap_pop();

        int64_t nx = (cur.side == 1) ? cur.x + 1 : cur.x - 1;
        long long next_v = 2LL*nx*nx + (long long)nx*cur.y - 2LL*cur.y*cur.y;
        if (next_v > 0) {
            Node nxt = { (seq_t)next_v, nx, cur.y, cur.side };
            heap_push(&nxt);
        }

        seq_t val = cur.val;
        if (val != last_val) {
            cache = (seq_t*)realloc(cache, (cache_sz + 1) * sizeof(seq_t));
            cache[cache_sz++] = val;
            last_val = val;
        }
        /* duplicates are skipped */
    }

    return cache[n - 1];
}
