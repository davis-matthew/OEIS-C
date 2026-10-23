/*
 * A035121  Numbers of the form x^2+82*y^2.
 * https://oeis.org/A035121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035121 – Numbers of the form x^2 + 82·y^2, in increasing order.
    Implementation: each fixed y gives an increasing sequence
        a_y(k) = (k)^2 + 82·y^2      (k ≥ 0)
    The algorithm merges these sequences with a min‑heap, discarding
    duplicate values.  The cache stores the results up to the required
    index so calls with arbitrary n are answered in O(n log n).    */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Node type for the priority queue                                   */
/* ------------------------------------------------------------------ */
struct Node {
    seq_t val;   /* current value  x^2 + 82·y^2 */
    seq_t base;  /* fixed part 82·y^2 */
    seq_t x;     /* current x  (≥0) */
};

/* ------------------------------------------------------------------ */
/* Heap helpers                                                        */
/* ------------------------------------------------------------------ */
static struct Node *heap = NULL;   /* pointer to the array */
static size_t heap_len = 0;        /* number of elements in the heap */
static size_t heap_cap = 0;        /* allocated capacity */

/* Grow the heap array to a new capacity.  The cast is required for C++. */
static void heap_resize(size_t new_cap) {
    heap = (struct Node *)realloc(heap, new_cap * sizeof(struct Node));
    heap_cap = new_cap;
}

/* Push a new node into the heap (min‑heap on node.val). */
static void heap_push(struct Node node) {
    if (heap_len + 1 > heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        if (new_cap < heap_len + 1) new_cap = heap_len + 1;
        heap_resize(new_cap);
    }
    heap[heap_len++] = node;

    /* sift up */
    size_t i = heap_len - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        struct Node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

/* Pop and return the smallest node from the heap. */
static struct Node heap_pop(void) {
    struct Node top = heap[0];
    heap[0] = heap[--heap_len];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        size_t r = l + 1;
        if (l >= heap_len) break;
        size_t smallest = l;
        if (r < heap_len && heap[r].val < heap[l].val) smallest = r;
        if (heap[smallest].val >= heap[i].val) break;
        struct Node tmp = heap[smallest];
        heap[smallest] = heap[i];
        heap[i] = tmp;
        i = smallest;
    }
    return top;
}

/* ------------------------------------------------------------------ */
/* Cache for already generated terms                                  */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;   /* array of generated terms */
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* Grow the cache array to a given capacity.  The cast is required for C++. */
static void cache_grow_to(size_t new_cap) {
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Helper to insert a new y‑sequence into the heap                     */
/* ------------------------------------------------------------------ */
static void insert_y(idx_t y) {
    seq_t ty = (seq_t)y;
    seq_t base = (seq_t)82 * ty * ty;          /* 82·y^2  */
    struct Node node = { .val = base, .base = base, .x = 0 };
    heap_push(node);
}

/* ------------------------------------------------------------------ */
/* Ensure that at least n terms are available in the cache.          */
/* ------------------------------------------------------------------ */
static void ensure_cache(idx_t n) {
    if (cache_size >= (size_t)n) return;

    /* initialise structures if this is the first call */
    if (cache_cap == 0) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (heap_cap == 0) {
        heap_cap = 256;
        heap = (struct Node *)malloc(heap_cap * sizeof(struct Node));
    }
    /* insert the first y‑sequence when the heap is empty */
    static idx_t next_y = 0;
    if (heap_len == 0) {
        insert_y(next_y);   /* y = 0 */
        next_y++;
    }

    /* variables for duplicate elimination */
    seq_t last_added = 0;
    int has_last = 0;

    while (cache_size < (size_t)n) {
        /* get the smallest value among all active sequences */
        struct Node cur = heap_pop();

        /* store it if it is not a duplicate */
        if (!has_last || cur.val != last_added) {
            if (cache_size == cache_cap)
                cache_grow_to(cache_cap * 2 + 1);
            cache[cache_size++] = cur.val;
            last_added = cur.val;
            has_last = 1;
        }

        /* push the next element of the same y‑sequence */
        struct Node nxt = cur;
        nxt.x = cur.x + 1;
        seq_t tx = nxt.x;
        nxt.val = nxt.base + tx * tx;
        heap_push(nxt);

        /* insert a new y‑sequence if we still need more candidates.
           We keep inserting up to at most n sequences – more than enough. */
        if (next_y < n) {
            insert_y(next_y);
            next_y++;
        }
    }
}

/* ------------------------------------------------------------------ */
/* External entry point required by the build system                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n<1, but safe */
    ensure_cache(n);
    return cache[n - 1];
}
