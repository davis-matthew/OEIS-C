/*
 * A212773  Amounts (in cents) of coins in denominations 1, 5, 10, 25, and 50
 *          (cents) which can consist of equal numbers of coins of all
 *          denominations present when two or more denominations are used (or
 *          none are used: term 0).
 * https://oeis.org/A212773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A212773 – Amounts of coins (1,5,10,25,50) with equal numbers of each
 *  denomination used (at least two denominations) or none.
 *
 *  The terms are the sorted union of all positive multiples of each of the
 *  26 sums of all non‑empty subsets of size ≥2 of {1,5,10,25,50}.
 *
 *  The algorithm uses a binary min‑heap that generates the multiples in
 *  order.  Duplicates (different subsets giving the same amount) are
 *  removed by comparing with the last emitted value.
 *  Complexity:  O(n log 26) ≈ O(n).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_BASE 26

/* Binary heap node: the base sum and the current multiple value. */
struct node {
    seq_t sum;      /* base sum (subset total) */
    seq_t current;  /* current multiple (tree element) */
};

/* heapify down: maintain min‑heap property. */
static void heapify_down(int idx, int size, struct node *h)
{
    while (1) {
        int left = idx << 1;
        int right = left + 1;
        int smallest = idx;

        if (left <= size && h[left].current < h[smallest].current)
            smallest = left;
        if (right <= size && h[right].current < h[smallest].current)
            smallest = right;
        if (smallest == idx)
            break;
        struct node tmp = h[idx];
        h[idx] = h[smallest];
        h[smallest] = tmp;
        idx = smallest;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Term 1 is 0 (no coins). */
    if (n == 1)
        return (seq_t)0;

    /* Target is the number of positive terms we need to generate. */
    idx_t target = n - 1;

    /* All subset sums of size ≥2 from {1,5,10,25,50}. */
    static const seq_t base_sums[NUM_BASE] = {
        (seq_t)6,  (seq_t)11, (seq_t)15, (seq_t)16, (seq_t)26, (seq_t)30,
        (seq_t)31, (seq_t)35, (seq_t)36, (seq_t)40, (seq_t)41, (seq_t)51,
        (seq_t)55, (seq_t)56, (seq_t)60, (seq_t)61, (seq_t)65, (seq_t)66,
        (seq_t)75, (seq_t)76, (seq_t)80, (seq_t)81, (seq_t)85, (seq_t)86,
        (seq_t)90, (seq_t)91
    };

    /* Max value representable in seq_t (used for overflow detection). */
    const seq_t max_val = (seq_t)-1;

    /* Min‑heap (1‑based). */
    struct node heap[NUM_BASE + 1];
    int heap_size = NUM_BASE;

    /* Initialise heap with the first multiple of each base sum. */
    for (int i = 0; i < NUM_BASE; ++i) {
        heap[i + 1].sum = base_sums[i];
        heap[i + 1].current = base_sums[i];
    }

    /* Build the heap. */
    for (int i = NUM_BASE >> 1; i >= 1; --i)
        heapify_down(i, heap_size, heap);

    seq_t last = 0;   /* last emitted positive term (distinct) */
    idx_t count = 0;  /* number of distinct positive terms emitted */

    while (count < target) {
        /* Extract minimum. */
        struct node node = heap[1];
        seq_t val = node.current;

        /* Emit if distinct. */
        if (val != last) {
            ++count;
            if (count == target)
                return val;
            last = val;
        }

        /* Prepare next multiple of this base sum. */
        seq_t new_val = node.current + node.sum;

        /* Check for overflow: in unsigned arithmetic overflow
           causes wrapped value < previous value. */
        if (new_val < node.current) {
            /* Remove this node from the heap. */
            if (heap_size == 1)
                break;   /* heap empty */
            heap[1] = heap[heap_size];
            --heap_size;
            heapify_down(1, heap_size, heap);
        } else {
            /* Replace root with the advanced node and restore heap. */
            node.current = new_val;
            heap[1] = node;
            heapify_down(1, heap_size, heap);
        }
    }

    /* If we reach here, n is larger than any value that fits in seq_t. */
    return max_val;
}
