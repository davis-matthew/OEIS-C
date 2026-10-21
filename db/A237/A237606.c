/*
 * A237606  Positive integers k such that x^2 - 8xy + y^2 + k = 0 has integer
 *          solutions.
 * https://oeis.org/A237606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237606: positive integers k such that
   x^2 - 8xy + y^2 + k = 0 has integer solutions.
   The set of such k is { 15*a^2 - b^2 : a ≥ 1, 0 ≤ b < sqrt(15)*a }.
   The function enumerates all these values in ascending order,
   removes duplicates and returns the n‑th term (offset 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Parameters – enough to generate >10 000 terms with a < 5000 */
#define MAX_A      5000   /* maximum a to pre‑seed the heap          */
#define MAX_TERM   20000  /* maximum number of terms we store       */

typedef struct {
    seq_t k;      /* current value of 15*a*a - b*b       */
    idx_t a;      /* fixed a for this sequence           */
    idx_t b;      /* current b ( ≥ 0 )                   */
} node_t;

/* Static storage for the sequence and the min‑heap          */
static seq_t   terms[MAX_TERM];
static size_t  terms_len = 0;              /* number of terms generated   */

static node_t  heap[MAX_A];
static size_t  heap_len  = 0;              /* current number of heap nodes*/

static bool    initialized = false;

/* Helper: swap two heap nodes */
static inline void heap_swap(size_t i, size_t j)
{
    node_t tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

/* Push a node into the heap (min‑heap on k) */
static void heap_push(node_t n)
{
    /* heap_len is always < MAX_A, so no bounds check needed */
    size_t idx = heap_len++;
    heap[idx] = n;
    /* sift up */
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].k <= heap[idx].k) break;
        heap_swap(parent, idx);
        idx = parent;
    }
}

/* Pop the minimum node from the heap */
static node_t heap_pop(void)
{
    node_t root = heap[0];
    heap[0] = heap[--heap_len];
    /* sift down */
    size_t idx = 0;
    while (true) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_len) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_len && heap[right].k < heap[left].k)
            smallest = right;
        if (heap[smallest].k >= heap[idx].k) break;
        heap_swap(idx, smallest);
        idx = smallest;
    }
    return root;
}

/* Initialise the heap with the smallest value for each a */
static void init(void)
{
    for (idx_t a = 1; a <= MAX_A; ++a) {
        seq_t a_sq  = (seq_t)a * (seq_t)a;
        seq_t valA  = (seq_t)15 * a_sq;
        seq_t bmax_seq = floor_sqrt(valA - (seq_t)1);
        idx_t bmax = (idx_t)bmax_seq;
        seq_t k = valA - (seq_t)bmax * (seq_t)bmax;
        node_t n;
        n.k = k;
        n.a = a;
        n.b = bmax;
        heap_push(n);
    }
    initialized = true;
}

/* Return the n‑th term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (!initialized) init();
    if (n == 0) return (seq_t)0;   /* past offset – undefined but harmless  */

    while (terms_len < n) {
        node_t cur = heap_pop();
        seq_t val  = cur.k;

        /* deduplicate – successive identical values are consecutive */
        if (terms_len == 0 || val != terms[terms_len - 1]) {
            if (terms_len < MAX_TERM) {
                terms[terms_len++] = val;
            }
        }

        /* push the next larger value for this a (if any) */
        if (cur.b > 0) {
            idx_t a   = cur.a;
            seq_t a_sq  = (seq_t)a * (seq_t)a;
            seq_t valA  = (seq_t)15 * a_sq;
            idx_t newb  = cur.b - 1;
            seq_t k_new = valA - (seq_t)newb * (seq_t)newb;
            node_t n;
            n.k = k_new;
            n.a = a;
            n.b = newb;
            heap_push(n);
        }
    }
    return terms[n - 1];
}
