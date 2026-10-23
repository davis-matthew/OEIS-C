/*
 * A033081  Numbers in which all pairs of consecutive base-10 digits differ by
 *          3.
 * https://oeis.org/A033081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 65, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A033081 – Numbers whose consecutive base‑10 digits differ by 3.  
    The function generates these numbers in increasing order using a
    min‑heap and returns the n‑th element (1‑based).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static state */
static seq_t *cache      = NULL;   /* array of generated numbers   */
static size_t cache_cap  = 0;      /* allocated capacity          */
static size_t cache_len  = 0;      /* number of elements stored   */

static seq_t *heap       = NULL;   /* binary min‑heap of numbers   */
static size_t heap_cap   = 0;      /* allocated capacity          */
static size_t heap_sz    = 0;      /* number of elements in heap  */

/* ---------- heap utilities ------------------------------------- */
static void heap_grow(void)
{
    size_t new_cap = heap_cap ? heap_cap << 1 : 8;
    heap = (seq_t *)realloc(heap, new_cap * sizeof(seq_t));
    heap_cap = new_cap;
}

static void heap_push(seq_t v)
{
    if (heap_sz + 1 > heap_cap) heap_grow();
    size_t i = heap_sz++;
    heap[i] = v;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= heap[i]) break;
        seq_t t = heap[p];
        heap[p] = heap[i];
        heap[i] = t;
        i = p;
    }
}

static seq_t heap_pop_min(void)
{
    seq_t min = heap[0];
    heap_sz--;
    if (heap_sz > 0) {
        heap[0] = heap[heap_sz];
        size_t i = 0;
        while (true) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_sz && heap[l] < heap[smallest]) smallest = l;
            if (r < heap_sz && heap[r] < heap[smallest]) smallest = r;
            if (smallest == i) break;
            seq_t t = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = t;
            i = smallest;
        }
    }
    return min;
}

/* ---------- main function ------------------------------------- */
seq_t a(idx_t n)
{
    /* initialize static tables on first use */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    if (heap == NULL) {
        heap_cap = 8;
        heap = (seq_t *)malloc(heap_cap * sizeof(seq_t));
        heap_sz = 0;
        /* start with all one‑digit positive numbers */
        for (seq_t d = 1; d <= 9; ++d) heap_push(d);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (heap_sz == 0) break;   /* safety – shouldn't happen */

        seq_t v = heap_pop_min();

        /* store the next term */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = v;

        /* generate successors: append a digit that differs by 3 */
        seq_t last = v % (seq_t)10;
        if (last <= 6) {                 /* last + 3 <= 9 */
            seq_t child = v * (seq_t)10 + (last + (seq_t)3);
            heap_push(child);
        }
        if (last >= 3) {                 /* last - 3 >= 0 */
            seq_t child = v * (seq_t)10 + (last - (seq_t)3);
            heap_push(child);
        }
    }

    /* cache[0] is a(1), so return cache[n‑1] */
    return cache[n - 1];
}
