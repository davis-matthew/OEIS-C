/*
 * A053990  Numbers of the form x*(x + 1)*y*(y + 1) ("bipronics") where x and
 *          y are distinct.
 * https://oeis.org/A053990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth smallest number of the form x*(x+1)*y*(y+1) with
 *            distinct non‑negative integers x != y. a(1) = 0, a(2) = 12, a(3)
 *            = 24, a(4) = 40, … (OEIS A053990)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for seq_t values */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *v1 = (const seq_t *)p1;
    const seq_t *v2 = (const seq_t *)p2;
    return ( *v1 < *v2 ) ? -1 : ( *v1 > *v2 ) ? 1 : 0;
}

/* A small min‑heap for (product, i, j) */
typedef struct {
    seq_t prod;
    idx_t i;
    idx_t j;
} HeapNode;

static void heap_insert(HeapNode *heap, size_t *size, HeapNode x)
{
    size_t idx = (*size)++;
    heap[idx] = x;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (cmp_seq(&(heap[parent].prod), &(heap[idx].prod)) <= 0)
            break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

static HeapNode heap_pop(HeapNode *heap, size_t *size)
{
    HeapNode top = heap[0];
    heap[0] = heap[--(*size)];
    size_t idx = 0;
    while (true) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < *size &&
            cmp_seq(&(heap[left].prod), &(heap[smallest].prod)) < 0)
            smallest = left;
        if (right < *size &&
            cmp_seq(&(heap[right].prod), &(heap[smallest].prod)) < 0)
            smallest = right;

        if (smallest == idx)
            break;
        HeapNode tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
    return top;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* value below the offset is irrelevant */

    if (n == 1)
        return 0;        /* a(1) = 0 */

    /* We need n-1 values after the initial 0.
       Use a heap over triangular numbers to generate them efficiently. */
    const size_t MAX_I = 500;   /* sufficient for n up to ~10000 */
    seq_t tri[MAX_I + 1];
    for (idx_t k = 0; k <= MAX_I; ++k)
        tri[k] = (seq_t)k * ((seq_t)k + 1);

    HeapNode heap[MAX_I];
    size_t heap_size = 0;

    /* Initialise heap with pairs (i, i+1) for i = 1 .. MAX_I-1 */
    for (idx_t i = 1; i < MAX_I; ++i) {
        idx_t j = i + 1;
        heap_insert(heap, &heap_size,
                    (HeapNode){ tri[i] * tri[j], i, j });
    }

    idx_t count_needed = n - 1;          /* excluding the initial 0 */
    idx_t result_seen = 0;
    seq_t last_output = (seq_t)-1;       /* value not present in the sequence */

    while (result_seen < count_needed) {
        HeapNode cur = heap_pop(heap, &heap_size);

        /* Skip duplicate products */
        if (cmp_seq(&(cur.prod), &last_output) != 0) {
            ++result_seen;
            last_output = cur.prod;
            if (result_seen == count_needed)
                return cur.prod;
        }

        /* Advance j for the same i */
        if (cur.j + 1 <= MAX_I) {
            idx_t new_j = cur.j + 1;
            HeapNode next = { tri[cur.i] * tri[new_j], cur.i, new_j };
            heap_insert(heap, &heap_size, next);
        }
    }

    /* Should never reach here */
    return 0;
}
