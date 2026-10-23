/*
 * A183991  Ranks of (even i)+(odd j)*r when all i+j*r are ranked, r=golden
 *          ratio, i>=0, j>=0.
 * https://oeis.org/A183991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A183991
 *
 *  For each pair of non‑negative integers (i,j) form the number
 *      v(i,j) = i + φ*j ,   φ = (1+√5)/2.
 *  All pairs are ordered increasingly by v(i,j).  The sequence
 *  contains the 1‑based ranks of those pairs for which i is even
 *  and j is odd.  That is:
 *      a(n) = the n‑th smallest integer k such that the
 *             k‑th element in the above ordering satisfies
 *             (i even) & (j odd).
 *
 *  The implementation uses an exact comparison of pairs based on the
 *  convergent 121393/75025 of φ.  The key of a node is
 *
 *      key(i,j) = Q*i + P*j           (P/Q ≈ φ)
 *
 *  The values of (P,Q) are chosen so that the error of the
 *  approximation is < 1/Q², guaranteeing correct ordering.
 */

#include <stdlib.h>

/* Rational approximation of the golden ratio */
#define PHI_P 121393ULL  /* numerator  (F_26) */
#define PHI_Q 75025ULL   /* denominator (F_25) */

/* Node kept in the min‑heap
 *   key = PHI_Q * i + PHI_P * j
 */
typedef struct {
    idx_t i;
    idx_t j;
    uint64_t key;
} heap_node;

/* heap insertion ------------------------------------------------------ */
static void heap_push(heap_node *heap, size_t *size, const heap_node *new_node)
{
    size_t idx = *size;
    heap[idx] = *new_node;
    (*size)++;

    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].key <= heap[idx].key)
            break;
        heap_node tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

/* heap pop ------------------------------------------------------------ */
static heap_node heap_pop(heap_node *heap, size_t *size)
{
    heap_node root = heap[0];
    (*size)--;
    if (*size > 0) {
        heap[0] = heap[*size];
        size_t idx = 0;
        while (1) {
            size_t left = (idx << 1) + 1;
            if (left >= *size)
                break;
            size_t right = left + 1;
            size_t smallest = idx;
            if (heap[left].key < heap[smallest].key)
                smallest = left;
            if (right < *size && heap[right].key < heap[smallest].key)
                smallest = right;
            if (smallest == idx)
                break;
            heap_node tmp = heap[idx];
            heap[idx] = heap[smallest];
            heap[smallest] = tmp;
            idx = smallest;
        }
    }
    return root;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)     /* offset starts at 1, but keep safe */
        return 0;

    /* we need enough columns to guarantee that the n‑th even–odd
       pair is reached.  For n up to 10 000 the bound 4*n+20 is ample. */
    idx_t Jmax = 4 * n + 20;
    if (Jmax < 1)
        Jmax = 1;

    /* priority queue initialised with the first element (i=0) from each
       column j = 0 … Jmax.  The heap will always contain Jmax+1 nodes. */
    size_t heap_cap = (size_t)Jmax + 1;
    heap_node *heap = (heap_node *)malloc(heap_cap * sizeof(heap_node));
    if (!heap)
        return 0;   /* allocation failure – unlikely in the tests */

    /* initialise heap */
    size_t heap_sz = 0;
    for (idx_t j = 0; j <= Jmax; ++j) {
        heap_node node;
        node.i   = 0;
        node.j   = j;
        node.key = PHI_Q * node.i + PHI_P * node.j;
        heap_push(heap, &heap_sz, &node);
    }

    /* iterate until the n‑th qualifying pair is found */
    idx_t rank   = 0;          /* current 1‑based rank in the full ordering */
    idx_t found  = 0;          /* number of even‑odd pairs seen so far */
    seq_t result = 0;

    while (1) {
        heap_node cur = heap_pop(heap, &heap_sz);
        rank++;

        /* check parity: i even, j odd */
        if (((cur.i & 1ULL) == 0ULL) && ((cur.j & 1ULL) == 1ULL)) {
            found++;
            if (found == n) {
                result = (seq_t)rank;
                break;
            }
        }

        /* push the next element from the same column */
        heap_node next;
        next.i   = cur.i + 1;
        next.j   = cur.j;
        next.key = PHI_Q * next.i + PHI_P * next.j;
        heap_push(heap, &heap_sz, &next);
    }

    free(heap);
    return result;
}
