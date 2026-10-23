/*
 * A139504  Primes of the form x^2 + 24x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A139504 – primes of the form x^2 + 24xy + y^2 for x,y ≥ 0.
   Computed by generating all such values up to a fixed limit, maintaining
   a min‑heap of the next value for each y, and testing each candidate
   for primality.  The search continues until the n‑th prime is found. */

#include <stdlib.h>
#include <stdbool.h>

/* External function from the utilities library for primality testing. */
#include "../utils/is_prime_big.h"

/* Struct for heap nodes. */
typedef struct {
    seq_t val;   /* value of x^2 + 24xy + y^2   */
    idx_t y;     /* current y                     */
    idx_t x;     /* current x (starting from y)   */
} Node;

/* Compute one such value.  Uses seq_t arithmetic to avoid overflow. */
static seq_t compute_value(idx_t x, idx_t y)
{
    seq_t xx = (seq_t)x * x;
    seq_t yy = (seq_t)y * y;
    seq_t cross = 24 * (seq_t)x * y;
    return xx + cross + yy;
}

/* Heap operations – a simple binary min‑heap. */
static void heap_push(Node *heap, idx_t *size, idx_t cap, Node n)
{
    idx_t i = *size;
    (*size)++;
    while (i > 0) {
        idx_t parent = (i - 1) >> 1;
        if (heap[parent].val <= n.val)
            break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = n;
}

static Node heap_pop(Node *heap, idx_t *size)
{
    Node root = heap[0];
    idx_t i = 0, child;
    idx_t last = (*size) - 1;
    Node lastNode = heap[last];
    (*size)--;
    while ((child = (i << 1) + 1) < *size) {
        if ((child + 1) < *size && heap[child + 1].val < heap[child].val)
            child++;
        if (lastNode.val <= heap[child].val)
            break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = lastNode;
    return root;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for the sequence but safe fallback */

    /* Fixed limit – large enough to contain at least 10000 primes of the form. */
    const seq_t LIMIT = 200000000ULL;

    /* Determine maximal y such that 26*y*y ≤ LIMIT.  This loop is very small. */
    idx_t y_max = 0;
    while (26 * y_max * y_max <= LIMIT)
        y_max++;

    /* Allocate heap for nodes – one initial node per y. */
    idx_t heap_cap = y_max;
    Node *heap = (Node *)malloc(heap_cap * sizeof(Node));
    if (!heap)
        return 0;  /* allocation failure – unlikely under constraints */

    idx_t heap_size = 0;

    /* Initialise heap with the first value for each y (x = y). */
    for (idx_t y = 0; y < y_max; ++y) {
        idx_t x = y;
        seq_t val = compute_value(x, y);
        if (val <= LIMIT)
            heap_push(heap, &heap_size, heap_cap, (Node){val, y, x});
    }

    /* Generate primes in ascending order. */
    idx_t prime_cnt = 0;
    while (prime_cnt < n) {
        Node cur = heap_pop(heap, &heap_size);
        if (is_prime_big(cur.val)) {
            prime_cnt++;
            if (prime_cnt == n) {
                seq_t ans = cur.val;
                free(heap);
                return ans;
            }
        }

        /* Advance x for the same y and push new value if within limit. */
        idx_t next_x = cur.x + 1;
        seq_t next_val = compute_value(next_x, cur.y);
        if (next_val <= LIMIT)
            heap_push(heap, &heap_size, heap_cap, (Node){next_val, cur.y, next_x});
        /* If next_val > LIMIT we simply do not push, effectively capping the series. */
    }

    /* Should never reach here for n up to requested size. */
    free(heap);
    return 0;
}
