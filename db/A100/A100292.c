/*
 * A100292  Numbers of the form a^5 + b^2 with a, b > 0.
 * https://oeis.org/A100292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th distinct integer of the form a^5 + b^2 with
 *            a,b>0 (sorted increasingly)
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   a5_plus_b2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/a5_plus_b2.h"

/* ---------- simple binary min‑heap for (value, a, b) pairs ---------- */
#define MAX_A   2000         /* maximum a considered (sufficient for n≈10000) */
#define HEAP_CAP (MAX_A * 2) /* ample capacity for the heap */

struct Node
{
    seq_t  val;
    uint64_t a;
    uint64_t b;
};

static struct Node heap[HEAP_CAP];
static size_t heapSize = 0;

static void node_swap(struct Node *x, struct Node *y)
{
    struct Node tmp = *x;
    *x = *y;
    *y = tmp;
}

static void heap_push(seq_t val, uint64_t a, uint64_t b)
{
    if (heapSize >= HEAP_CAP) return;          /* safety guard */
    size_t i = heapSize++;
    heap[i].val = val;
    heap[i].a   = a;
    heap[i].b   = b;

    /* sift up */
    while (i > 0)
    {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        node_swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

static struct Node heap_pop(void)
{
    struct Node top = heap[0];
    heapSize--;
    if (heapSize > 0)
    {
        heap[0] = heap[heapSize];
        size_t i = 0;
        while (1)
        {
            size_t left  = (i << 1) + 1;
            size_t right = left + 1;
            size_t smallest = i;

            if (left < heapSize && heap[left].val < heap[smallest].val)
                smallest = left;
            if (right < heapSize && heap[right].val < heap[smallest].val)
                smallest = right;
            if (smallest == i) break;
            node_swap(&heap[i], &heap[smallest]);
            i = smallest;
        }
    }
    return top;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but guard for completeness */

    static bool seenA[MAX_A + 1];
    memset(seenA, 0, sizeof(seenA));

    heapSize = 0;
    seenA[1] = true;
    heap_push(a5_plus_b2(1, 1), 1, 1);

    seq_t last = (seq_t)0;
    idx_t outCnt = 0;

    while (1)
    {
        struct Node cur = heap_pop();

        if (outCnt == 0 || cur.val != last)
        {
            ++outCnt;
            last = cur.val;
            if (outCnt == n) return last;
        }

        /* push next b for the same a */
        heap_push(a5_plus_b2(cur.a, cur.b + 1), cur.a, cur.b + 1);

        /* push next a (with b=1) if not already pending */
        uint64_t nextA = cur.a + 1;
        if (nextA <= MAX_A && !seenA[nextA])
        {
            seenA[nextA] = true;
            heap_push(a5_plus_b2(nextA, 1), nextA, 1);
        }
    }
}
