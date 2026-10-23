/*
 * A025670  Exponent of 7 (value of i) in n-th number of form 7^i*9^j.
 * https://oeis.org/A025670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025670: a(n) = exponent of 7 in the n‑th smallest
   integer of the form 7^i * 9^j (sorted by value). */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* heap node ---------------------------------------------------------- */
typedef struct {
    uint32_t i;      /* exponent of 7   */
    uint32_t j;      /* exponent of 9   */
    double   val;    /* i*log7 + j*log9 (for ordering) */
} node_t;

/* --------------------------------------------------------------------- */
static void heap_push(node_t *heap, size_t *size, size_t cap, node_t n)
{
    size_t pos = (*size)++;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].val <= n.val) break;
        heap[pos] = heap[parent];
        pos = parent;
    }
    heap[pos] = n;
}

static node_t heap_pop(node_t *heap, size_t *size)
{
    node_t top = heap[0];
    node_t last = heap[--(*size)];
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        if (left >= *size) break;
        size_t right = left + 1;
        size_t child = (right < *size && heap[right].val < heap[left].val) ?
                       right : left;
        if (last.val <= heap[child].val) break;
        heap[pos] = heap[child];
        pos = child;
    }
    heap[pos] = last;
    return top;
}

/* --------------------------------------------------------------------- */
/* small open‑addressing hash table for int64 keys (local to a(n))   */
typedef struct {
    uint64_t key;
    bool     used;
} vis_entry_t;

static void vis_init(vis_entry_t *tbl, size_t cap, uint64_t *size)
{
    *size = 0;
    for (size_t i = 0; i < cap; ++i) tbl[i].used = false;
}

static bool vis_insert(vis_entry_t *tbl, size_t cap, uint64_t key, uint64_t *size)
{
    size_t idx = key & (cap - 1);
    while (tbl[idx].used) {
        if (tbl[idx].key == key) return false;          /* already seen */
        idx = (idx + 1) & (cap - 1);
    }
    tbl[idx].used = true;
    tbl[idx].key  = key;
    ++(*size);
    return true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset is 1; undefined for n<1 */

    /* log constants (2 pre‑evaluated floating‑point constants) */
    static const double log7 = log(7.0);
    static const double log9 = log(9.0);

    /* local heap ---------------------------------------------------- */
    size_t heap_cap = 1;
    while (heap_cap < 2 * (size_t)n + 4) heap_cap <<= 1;
    node_t *heap = (node_t *)malloc(heap_cap * sizeof(node_t));
    if (!heap) return 0;            /* allocation failure */
    size_t heap_size = 0;

    /* local visited set -------------------------------------------- */
    size_t tbl_cap = 1;
    while (tbl_cap < 4 * (size_t)n + 16) tbl_cap <<= 1;
    vis_entry_t *vis_tbl = (vis_entry_t *)malloc(tbl_cap * sizeof(vis_entry_t));
    if (!vis_tbl) { free(heap); return 0; }
    uint64_t vis_used = 0;
    vis_init(vis_tbl, tbl_cap, &vis_used);

    /* seed heap with (0,0) ------------------------------------------------ */
    uint64_t key0 = 0; /* ((uint64_t)0 << 32) | 0 */
    vis_insert(vis_tbl, tbl_cap, key0, &vis_used);
    heap_push(heap, &heap_size, heap_cap, (node_t){0, 0, 0.0});

    seq_t answer = 0;

    for (idx_t step = 1; step <= n; ++step) {
        node_t cur = heap_pop(heap, &heap_size);
        answer = (seq_t)cur.i;

        if (step == n) break;

        /* generate successors */
        uint32_t i = cur.i;
        uint32_t j = cur.j;

        uint64_t key1 = ((uint64_t)(i + 1) << 32) | (uint64_t)j;
        if (vis_insert(vis_tbl, tbl_cap, key1, &vis_used)) {
            heap_push(heap, &heap_size, heap_cap,
                      (node_t){i + 1, j, cur.val + log7});
        }

        uint64_t key2 = ((uint64_t)i << 32) | (uint64_t)(j + 1);
        if (vis_insert(vis_tbl, tbl_cap, key2, &vis_used)) {
            heap_push(heap, &heap_size, heap_cap,
                      (node_t){i, j + 1, cur.val + log9});
        }
    }

    free(heap);
    free(vis_tbl);
    return answer;
}
