/*
 * A069858  1/n has period 4 in base 10.
 * https://oeis.org/A069858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = numbers of the form a*2^i*5^j with * a ∈
 *            {101,303,909,1111,3333,9999}
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- min‑heap helpers ----------------------------------------- */
static seq_t *heap = NULL;
static size_t heap_sz = 0, heap_cap = 0;

static void heap_push(seq_t v)
{
    if (heap_sz == heap_cap) {
        size_t new_cap = (heap_cap == 0) ? 64 : (heap_cap << 1);
        seq_t *tmp = (seq_t *)realloc(heap, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_sz++;
    heap[i] = v;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p] <= heap[i]) break;
        seq_t t = heap[p]; heap[p] = heap[i]; heap[i] = t;
        i = p;
    }
}

static seq_t heap_pop(void)
{
    if (heap_sz == 0) abort();          /* should not happen */
    seq_t min_val = heap[0];
    heap[0] = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_sz && heap[l] < heap[smallest]) smallest = l;
        if (r < heap_sz && heap[r] < heap[smallest]) smallest = r;
        if (smallest == i) break;
        seq_t t = heap[i]; heap[i] = heap[smallest]; heap[smallest] = t;
        i = smallest;
    }
    return min_val;
}

/* ---------- cache of generated terms ------------------------------ */
static seq_t *cache = NULL;
static size_t cache_sz = 0, cache_cap = 0;
static bool initialized = false;

/* ---------- the main sequence function ----------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety guard, offset is 1 */

    if (!initialized) {
        heap_cap = 64;
        heap = (seq_t *)malloc(heap_cap * sizeof(seq_t));

        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        /* seed the heap with the six base constants */
        const seq_t bases[6] = {101, 303, 909, 1111, 3333, 9999};
        for (int i = 0; i < 6; ++i) heap_push(bases[i]);

        initialized = true;
    }

    static seq_t last_val = 0;
    static bool first = true;

    while (cache_sz < n) {
        seq_t val = heap_pop();

        /* skip duplicate values that appear consecutively */
        if (!first && val == last_val) {
            /* children of a duplicate are already inserted by the
             * first occurrence so we do nothing */
            last_val = val;
            continue;
        }

        /* store the unique value */
        if (cache_sz == cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = val;
        last_val = val;
        first = false;

        /* generate next multiples */
        heap_push(val * (seq_t)2);
        heap_push(val * (seq_t)5);
    }

    return cache[n - 1];
}
