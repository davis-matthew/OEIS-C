/*
 * A051207  Numbers of the form x^2-5^y >= 0.
 * https://oeis.org/A051207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   ceil_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051207: numbers of the form x^2 - 5^y ≥ 0, sorted ascending */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ceil_sqrt_seq.h"

/* Node used in the min‑heap: the current value together with its
   exponent y and the current x for this y. */
typedef struct {
    seq_t n;   /* current value x^2 – 5^y */
    idx_t y;   /* exponent y */
    seq_t x;   /* current x */
} HeapNode;

#define MAX_Y     128     /* enough for all required y values  */
#define MAX_CACHE 20000   /* cache up to this many terms (covers n≈10 000) */

/* Tables and state -------------------------- */
static seq_t            pow5[MAX_Y];
static size_t           pow5_len;
static HeapNode         heap[MAX_Y];
static size_t           heap_sz;
static seq_t            cache[MAX_CACHE];
static size_t           cache_sz;
static bool             initialized = false;

/* ----------- heap helpers ----------------- */
static void heap_push(HeapNode *h, size_t *size, const HeapNode node)
{
    size_t i = *size;
    h[i] = node;
    (*size)++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h[parent].n <= h[i].n) break;
        HeapNode tmp = h[parent]; h[parent] = h[i]; h[i] = tmp;
        i = parent;
    }
}

static HeapNode heap_pop(HeapNode *h, size_t *size)
{
    HeapNode ret = h[0];
    (*size)--;
    h[0] = h[*size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1, r = l + 1, smallest = i;
        if (l < *size && h[l].n < h[smallest].n) smallest = l;
        if (r < *size && h[r].n < h[smallest].n) smallest = r;
        if (smallest == i) break;
        HeapNode tmp = h[i]; h[i] = h[smallest]; h[smallest] = tmp;
        i = smallest;
    }
    return ret;
}

/* --------- sequence initialization --------- */
static void init_sequence(void)
{
    /* powers of 5 */
    pow5[0] = (seq_t)1;
    size_t i = 1;
    for (; i < MAX_Y; ++i) {
        seq_t next = pow5[i - 1] * (seq_t)5;
        if (next <= pow5[i - 1]) break;   /* overflow for fixed‑width types */
        pow5[i] = next;
    }
    pow5_len = i;

    /* build heap with initial values for each y */
    heap_sz = 0;
    for (size_t y = 0; y < pow5_len; ++y) {
        seq_t x_start = ceil_sqrt_seq(pow5[y]);
        seq_t n_start = x_start * x_start - pow5[y];
        HeapNode node = { n_start, (idx_t)y, x_start };
        heap_push(heap, &heap_sz, node);
    }

    /* cache for generated terms */
    cache_sz = 0;

    initialized = true;
}

/* ------------ main sequence function ------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, fallback */

    if (!initialized) init_sequence();

    while (cache_sz < (size_t)n) {
        if (heap_sz == 0) break;   /* no more values (should not happen) */

        HeapNode node = heap_pop(heap, &heap_sz);
        seq_t val = node.n;

        /* store unique values only */
        if (cache_sz == 0 || val != cache[cache_sz - 1]) {
            cache[cache_sz++] = val;
        }

        /* next value for the same y: (x+1)^2 - 5^y */
        seq_t inc     = (seq_t)2 * node.x + (seq_t)1;  /* (x+1)^2 - x^2 */
        seq_t next_n  = node.n + inc;
        if (next_n < node.n) continue;                /* unsigned wrap */

        node.x = node.x + (seq_t)1;
        node.n = next_n;
        heap_push(heap, &heap_sz, node);
    }

    return cache[n - 1];
}
