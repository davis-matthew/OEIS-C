/*
 * A182396  a(n) is the least number not occurring earlier such that
 *          neighboring digits sum to 1 or 10.
 * https://oeis.org/A182396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182396: generate the least number not used earlier such that
   every adjacent pair of digits sums to 1 or 10.  Starting with a(0)=0,
   maintain two min‑heaps of all unused admissible numbers that start with
   digit 1 and digit 9, respectively.  For each new term, pop the smallest
   number from the heap whose starting digit is forced by the last digit of
   the previous term (if the previous last digit is 1, the next number
   must start with 9, otherwise it must start with 1).  Children of a
   number are formed by appending a digit that preserves the adjacency
   rule; they are inserted back into the same heap.  Results are cached
   for arbitrary index access. */
#include <stdlib.h>
#include <stdbool.h>

/* heap used to hold admissible numbers */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} Heap;

/* initialise an empty heap */
static void heap_init(Heap *h)
{
    h->size = 0;
    h->cap  = 8;
    h->data = (seq_t *)malloc(h->cap * sizeof(seq_t));
}

/* bubble the new element up to maintain min‑heap property */
static void heap_push(Heap *h, seq_t v)
{
    if (h->size == h->cap) {
        h->cap *= 2;
        h->data = (seq_t *)realloc(h->data, h->cap * sizeof(seq_t));
    }
    size_t i = h->size++;
    h->data[i] = v;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[i] >= h->data[p]) break;
        seq_t t = h->data[i];
        h->data[i] = h->data[p];
        h->data[p] = t;
        i = p;
    }
}

/* pop the minimum element from the heap */
static seq_t heap_pop(Heap *h)
{
    seq_t res = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    for (;;) {
        size_t l = i * 2 + 1;
        size_t r = l + 1;
        size_t m = i;
        if (l < h->size && h->data[l] < h->data[m]) m = l;
        if (r < h->size && h->data[r] < h->data[m]) m = r;
        if (m == i) break;
        seq_t t = h->data[i];
        h->data[i] = h->data[m];
        h->data[m] = t;
        i = m;
    }
    return res;
}

/* static state for the sequence */
static Heap heap_start1, heap_start9;
static bool heaps_inited = false;
static unsigned char last_digit = 0;

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* ensure the cache can hold value at index n */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while ((size_t)n >= new_cap) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* initialise heaps and cache */
static void init_sequence(void)
{
    heap_init(&heap_start1);
    heap_init(&heap_start9);

    heap_push(&heap_start1, (seq_t)1);
    heap_push(&heap_start9, (seq_t)9);

    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache[0] = (seq_t)0;          /* a(0) */
    cache_size = 1;
    last_digit = 0;

    heaps_inited = true;
}

seq_t a(idx_t n)
{
    if (!heaps_inited) init_sequence();

    ensure_cache_capacity(n);

    while (cache_size <= n) {
        /* pick the appropriate heap based on the last digit of the previous term */
        Heap *src = (last_digit == 1) ? &heap_start9 : &heap_start1;
        seq_t val = heap_pop(src);       /* new term */
        cache[cache_size++] = val;

        int cur_last = (int)(val % (seq_t)10);
        last_digit = (unsigned char)cur_last;   /* update for next step */

        /* generate and push children of the new term */
        if (cur_last == 1) {
            seq_t child0 = val * (seq_t)10 + (seq_t)0;
            seq_t child9 = val * (seq_t)10 + (seq_t)9;
            heap_push(src, child0);
            heap_push(src, child9);
        } else {   /* cur_last is 0 or 9 */
            seq_t child1 = val * (seq_t)10 + (seq_t)1;
            heap_push(src, child1);
        }
    }

    return cache[n];
}
