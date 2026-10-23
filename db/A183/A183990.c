/*
 * A183990  Indices of pairs with (odd i, even j) in list of pairs (i,j)
 *          ordered by value of i+j*r, where r=(1+sqrt(5))/2, i>=0, j>=0.
 * https://oeis.org/A183990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the rank of the nth pair (i,j) with i odd and j even,
 *            where all pairs (i,j), i>=0, j>=0, are sorted by increasing
 *            value i + φ*j, φ = (1+√5)/2. The function generates pairs in
 *            order using a min‑heap and extracts the ranks of qualifying
 *            pairs
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* External integer types supplied by the build */
typedef uint64_t idx_t;     /* provided by the prelude, do not redeclare */

/* ----------------------------------------------------- */
/*  Heap of pairs (i,j) ordered by weight i + φ*j        */
/* ----------------------------------------------------- */
typedef struct {
    uint64_t i;
    uint64_t j;
    double w;
} heap_node;

typedef struct {
    heap_node *data;
    size_t   size;
    size_t   cap;
} heap_t;

static heap_t heap = {NULL, 0, 0};

static void heap_init(void)
{
    heap.cap = 64;
    heap.data = (heap_node *)malloc(heap.cap * sizeof(heap_node));
}

static void heap_resize(void)
{
    size_t new_cap = heap.cap ? heap.cap * 2U : 64U;
    heap.data = (heap_node *)realloc(heap.data, new_cap * sizeof(heap_node));
    heap.cap = new_cap;
}

static void heap_push(heap_node n)
{
    if (heap.size == heap.cap)
        heap_resize();
    size_t idx = heap.size++;
    heap.data[idx] = n;
    /* sift-up */
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap.data[parent].w <= heap.data[idx].w)
            break;
        heap_node tmp = heap.data[parent];
        heap.data[parent] = heap.data[idx];
        heap.data[idx] = tmp;
        idx = parent;
    }
}

static heap_node heap_pop_min(void)
{
    heap_node ret = heap.data[0];
    if (--heap.size == 0)
        return ret;
    heap.data[0] = heap.data[heap.size];
    /* sift-down */
    size_t idx = 0;
    for (;;) {
        size_t left = (idx << 1) + 1U;
        size_t right = left + 1U;
        size_t smallest = idx;
        if (left < heap.size && heap.data[left].w < heap.data[smallest].w)
            smallest = left;
        if (right < heap.size && heap.data[right].w < heap.data[smallest].w)
            smallest = right;
        if (smallest == idx)
            break;
        heap_node tmp = heap.data[idx];
        heap.data[idx] = heap.data[smallest];
        heap.data[smallest] = tmp;
        idx = smallest;
    }
    return ret;
}

/* ----------------------------------------------------- */
/*  Cache of ranks of qualifying pairs (odd i, even j)   */
/* ----------------------------------------------------- */
static seq_t *rank_cache = NULL;   /* cache of a(1), a(2), ...  */
static size_t cache_used = 0;      /* number of stored ranks */
static size_t cache_cap  = 0;      /* cache capacity */

/* ----------------------------------------------------- */
/*  Generation state                                     */
/* ----------------------------------------------------- */
static idx_t current_rank = 0;          /* total number of processed pairs */
static idx_t next_j_to_insert = 1;     /* next j not yet inserted into heap */
static const double PHI = 1.618033988749894848204586; /* (1+sqrt5)/2 */

/* ----------------------------------------------------- */
/*  Ensure cache contains at least n entries (n>=1)     */
/* ----------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (rank_cache == NULL) {
        /* First time initialisation */
        cache_cap = 64;
        rank_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        heap_init();
        /* Insert the very first pair (0,0) */
        heap_node first = {0ULL, 0ULL, 0.0};
        heap_push(first);
        /* next_j_to_insert already 1 */
    }

    while (cache_used < (size_t)n) {
        /* Get next pair in sorted order */
        heap_node cur = heap_pop_min();
        ++current_rank;

        /* If (i odd) and (j even), store its rank */
        if ( (cur.i & 1ULL) && !(cur.j & 1ULL) ) {
            if (cache_used == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 64U;
                rank_cache = (seq_t *)realloc(rank_cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            rank_cache[cache_used++] = (seq_t)current_rank;   /* cast to seq_t */
        }

        /* Insert successor on the same j line */
        heap_node succ = {cur.i + 1ULL, cur.j, cur.w + 1.0};
        heap_push(succ);

        /* Insert the next new j line (0, next_j) if not yet inserted */
        heap_node newj = {0ULL, next_j_to_insert, PHI * (double)next_j_to_insert};
        heap_push(newj);
        ++next_j_to_insert;
    }
}

/* ----------------------------------------------------- */
/*  Public function required by the build framework    */
/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, return 0 by convention */
    ensure_cache(n);
    return rank_cache[n - 1];
}
