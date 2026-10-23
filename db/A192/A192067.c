/*
 * A192067  Monotonically ordered sums 2^i+3*2^(j-1), for i>=1, j>=1.
 * https://oeis.org/A192067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest distinct value of 2^i + 3*2^(j-1) for
 *            i≥1, j≥1
 * Range:     u8 none, u16 none, u32 none, u64 n <= 58, u128 none, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

typedef struct {
    idx_t i;
    idx_t k;
    seq_t val;
} node_t;

static void heap_swap(node_t *a, node_t *b) {
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static void sift_up(node_t *heap, size_t heap_size, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= heap[idx].val)
            break;
        heap_swap(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

static void sift_down(node_t *heap, size_t heap_size, size_t idx) {
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_size)
            break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (heap[smallest].val >= heap[idx].val)
            break;
        heap_swap(&heap[smallest], &heap[idx]);
        idx = smallest;
    }
}

seq_t a(idx_t n) {
    /* Limits that avoid overflow on 64‑bit builds. */
    static const idx_t I_LIMIT = (sizeof(seq_t) <= 8) ? 63 : 200;
    static const idx_t K_LIMIT = (sizeof(seq_t) <= 8) ? 62 : 200;
    const size_t K_MAX = (size_t)(K_LIMIT + 1);

    node_t heap[K_MAX];
    size_t heap_size = 0;

    /* initialise heap with all k = 0 .. K_LIMIT, i = 1 */
    for (idx_t k = 0; k <= K_LIMIT; ++k) {
        seq_t val = pow_seq((seq_t)2, (idx_t)1) + ((seq_t)3 * pow_seq((seq_t)2, k));
        heap[heap_size++] = (node_t){ (idx_t)1, k, val };
    }
    /* build min‑heap */
    for (size_t i = 0; i < heap_size / 2; ++i)
        sift_down(heap, heap_size, (heap_size / 2 - 1) - i);

    seq_t last_val = 0;
    bool have_last = false;
    idx_t count = 0;

    while (1) {
        node_t top = heap[0];
        /* pop */
        heap[0] = heap[--heap_size];
        sift_down(heap, heap_size, 0);

        /* count distinct values */
        if (!have_last || top.val != last_val) {
            last_val = top.val;
            have_last = true;
            ++count;
            if (count == n)
                return last_val;
        }

        /* push next value with the same k */
        idx_t next_i = top.i + 1;
        if (next_i <= I_LIMIT) {
            seq_t val = pow_seq((seq_t)2, next_i) + ((seq_t)3 * pow_seq((seq_t)2, top.k));
            heap[heap_size++] = (node_t){ next_i, top.k, val };
            sift_up(heap, heap_size, heap_size - 1);
        }
        /* the heap will never become empty for valid n within the defined limits */
    }
}
