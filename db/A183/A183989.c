/*
 * A183989  Ranks of (odd i)+(odd j)*r when all i+j*r are ranked, where
 *          r=golden ratio, i>=0, j>=0.
 * https://oeis.org/A183989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A183989:
   a(n) = the rank (starting at 1) of the n‑th pair (i,j) with i and j
   both odd in the ordering of all pairs (i,j) with i,j>=0 by increasing
   value of i + φ·j, where φ is the golden ratio.
   The sequence is generated lazily and memoised: calling a(n) in any order
   will return the correct value.  Each new n expands the calculation as
   needed.  The algorithm uses a min‑heap to enumerate the points in order
   and keeps only the odds needed for the sequence.  No floating‑point
   arithmetic is performed on the sequence values themselves – only as a
   comparator for heap ordering. */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct {
    idx_t i;
    idx_t j;
} node_t;

/* The golden ratio φ = (1 + sqrt(5)) / 2, computed as a long double. */
static const long double R = (1.0L + sqrtl(5.0L)) / 2.0L;

/* --- Heap data structure ---------------------------------------------- */
static node_t *heap = NULL;        /* 1‑based heap array */
static size_t heap_alloc = 0;
static size_t heap_sz     = 0;
static idx_t  next_insert_j = 0;   /* next j to push into heap */

/* Grow the heap array when needed. */
static void heap_grow(size_t mincap)
{
    if (heap_alloc >= mincap) return;
    size_t newcap = heap_alloc ? heap_alloc * 2 : 16;
    while (newcap < mincap) newcap <<= 1;
    node_t *newheap = (node_t *)realloc(heap, newcap * sizeof(node_t));
    assert(newheap != NULL);
    heap = newheap;
    heap_alloc = newcap;
}

/* Compare two nodes by the value i + R*j.  Return <0, 0, >0. */
static int cmp_node(const node_t *a, const node_t *b)
{
    long double va = (long double)a->i + R * (long double)a->j;
    long double vb = (long double)b->i + R * (long double)b->j;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0; /* practically never equal for distinct (i,j) */
}

static void heap_push(node_t v)
{
    heap_grow(heap_sz + 2);          /* +1 for 1‑based, +1 extra */
    size_t k = ++heap_sz;
    heap[k] = v;
    while (k > 1) {
        size_t parent = k >> 1;
        if (cmp_node(&heap[k], &heap[parent]) < 0) {
            node_t tmp = heap[k];
            heap[k] = heap[parent];
            heap[parent] = tmp;
            k = parent;
        } else break;
    }
}

static void heap_pop(node_t *out)
{
    *out = heap[1];
    heap[1] = heap[heap_sz--];
    size_t k = 1;
    while (1) {
        size_t left = k << 1;
        size_t right = left + 1;
        if (left > heap_sz) break;
        size_t child = left;
        if (right <= heap_sz && cmp_node(&heap[right], &heap[left]) < 0)
            child = right;
        if (cmp_node(&heap[child], &heap[k]) < 0) {
            node_t tmp = heap[child];
            heap[child] = heap[k];
            heap[k] = tmp;
            k = child;
        } else break;
    }
}

/* Push all j values from next_insert_j up to (but not including) limit. */
static void ensure_rows(idx_t limit)
{
    while (next_insert_j < limit) {
        heap_push((node_t){0, next_insert_j});
        ++next_insert_j;
    }
}

/* --- Sequence storage --------------------------------------------------- */
static seq_t *odd_ranks = NULL;     /* array of computed ranks (1‑based) */
static size_t odd_cap  = 0;
static size_t odd_cnt  = 0;

/* Grow odd_ranks array. */
static void odd_ensure(size_t need)
{
    if (odd_cap >= need) return;
    size_t newcap = odd_cap ? odd_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *newarr = (seq_t *)realloc(odd_ranks, newcap * sizeof(seq_t));
    assert(newarr != NULL);
    odd_ranks = newarr;
    odd_cap = newcap;
}

/* --- Main function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; ignore n = 0. */
    assert(n >= 1);

    /* Continue generating until we have at least n odd‑odd ranks. */
    while (odd_cnt < (size_t)n) {
        /* Estimate how many rows (j values) are needed for the next batch. */
        /* A safe overestimate: J_max ≈ rank_counter / 0.6 */
        idx_t needed_rows = (idx_t)( (double)(odd_cnt + 1) * 2.0 );
        if (needed_rows < next_insert_j + 1000) needed_rows = next_insert_j + 1000;
        ensure_rows(needed_rows);

        /* Pop the next minimal point. */
        node_t cur;
        heap_pop(&cur);
        static idx_t rank_counter = 0;
        ++rank_counter;   /* rank of this pair (starting at 1) */

        /* Store if i and j are both odd. */
        if ( (cur.i & 1ULL) && (cur.j & 1ULL) ) {
            odd_ensure(odd_cnt + 1);
            odd_ranks[odd_cnt++] = (seq_t)rank_counter;
        }

        /* Push the next point in this column j. */
        heap_push((node_t){cur.i + 1, cur.j});
    }

    return odd_ranks[n - 1];
}
