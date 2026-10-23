/*
 * A020681  Numbers of form 3 x^2 + 10 y^2.
 * https://oeis.org/A020681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers of the form 3*x^2 + 10*y^2, sorted ascending. */
#include <stdlib.h>
#include <stdbool.h>

#define YMAX 8192           /* number of y‑lines kept in the heap */

/* Element of the heap: one growing sequence for fixed y. */
struct node_t {
    seq_t val;      /* current value 3*x^2 + 10*y^2 */
    unsigned x;     /* current x for this fixed y */
    unsigned y;     /* fixed y */
};

static struct node_t heap[YMAX];
static unsigned heap_size = 0;
static bool heap_inited = false;

/* Static cache of distinct sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache can hold at least n items. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n * 2;
        void *tmp = realloc(cache, sizeof(seq_t) * new_cap);
        /* cast for C/C++ compatibility */
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }
}

/* Append a value to the cache if it is distinct from the last entry. */
static void append_cache(seq_t v)
{
    if (cache_len == 0 || cache[cache_len - 1] != v)
        cache[cache_len++] = v;
}

/* Initialise the heap with the first value (x = 0) for each y‑line. */
static void init_heap(void)
{
    for (unsigned y = 0; y < YMAX; ++y) {
        heap[y].val = (seq_t)10 * (seq_t)y * (seq_t)y;  /* 3*0^2 + 10*y^2 */
        heap[y].x   = 0;
        heap[y].y   = y;
    }
    heap_size = YMAX;
    heap_inited = true;
}

/* Find the index of the smallest value in the heap. */
static unsigned find_min_index(void)
{
    unsigned min_idx = 0;
    seq_t min_val = heap[0].val;
    for (unsigned i = 1; i < heap_size; ++i) {
        if (heap[i].val < min_val) {
            min_val = heap[i].val;
            min_idx = i;
        }
    }
    return min_idx;
}

/* Return the n-th smallest distinct number of the form 3*x^2 + 10*y^2. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* safety – offset is 1 */

    ensure_cache(n);

    if (!heap_inited)
        init_heap();

    while (cache_len < (size_t)n) {
        /* Extract the minimal node */
        unsigned mi = find_min_index();
        struct node_t node = heap[mi];

        /* Remove it from the heap by replacing with the last element */
        heap[mi] = heap[heap_size - 1];
        --heap_size;

        /* Store the value if it is new */
        append_cache(node.val);

        /* Insert the next value from the same y‑line */
        unsigned new_x = node.x + 1;
        seq_t new_val = (seq_t)3 * (seq_t)new_x * (seq_t)new_x
                      + (seq_t)10 * (seq_t)node.y * (seq_t)node.y;
        heap[heap_size].val = new_val;
        heap[heap_size].x   = new_x;
        heap[heap_size].y   = node.y;
        ++heap_size;
    }

    return cache[n - 1];
}
