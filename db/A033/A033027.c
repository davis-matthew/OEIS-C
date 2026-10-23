/*
 * A033027  Numbers whose base-14 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A033027 – numbers whose base‑14 expansion has no run of digits
 * with length < 2.  The following code computes the sequence by
 * generating all valid base‑14 strings in ascending numeric order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t *heap = NULL;
static size_t heap_len = 0, heap_cap = 0;
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

static void heap_push(seq_t x) {
    if (heap_len == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 16;
        heap = (seq_t *)realloc(heap, heap_cap * sizeof(seq_t));
    }
    heap[heap_len] = x;
    size_t i = heap_len++;
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (heap[p] <= heap[i]) break;
        seq_t t = heap[p]; heap[p] = heap[i]; heap[i] = t;
        i = p;
    }
}

static seq_t heap_pop_min(void) {
    seq_t min = heap[0];
    heap[0] = heap[--heap_len];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1;
        if (l >= heap_len) break;
        size_t r = l + 1;
        size_t child = (r < heap_len && heap[r] < heap[l]) ? r : l;
        if (heap[i] <= heap[child]) break;
        seq_t t = heap[i]; heap[i] = heap[child]; heap[child] = t;
        i = child;
    }
    return min;
}

static void cache_push(seq_t val) {
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

static void init(void) {
    initialized = true;
    /* initial numbers: two identical digits (1‑1 … 13‑13) */
    for (int d = 1; d <= 13; ++d) {
        seq_t val = (seq_t)d * ((seq_t)15); /* d*14 + d */
        heap_push(val);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1; value undefined for n<1 */
    if (!initialized) init();

    while (cache_len < n) {
        seq_t v = heap_pop_min();    /* next term */
        cache_push(v);

        seq_t d = v % ((seq_t)14);   /* last digit */
        seq_t same = v * ((seq_t)14) + d;
        heap_push(same);

        for (int e = 0; e < 14; ++e) {
            if ((seq_t)e == d) continue;          /* blocks must use new digit */
            seq_t new_block = v * ((seq_t)196) + ((seq_t)e * 14) + e;
            heap_push(new_block);
        }
    }
    return cache[n - 1];
}
