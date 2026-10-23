/*
 * A009012  Ordered long legs of Pythagorean triangles.
 * https://oeis.org/A009012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ordered long legs of Pythagorean triangles (OEIS A009012)
   a(n) is the n‑th smallest positive integer that occurs as the longer
   leg of a (primitive or non‑primitive) Pythagorean triple.
   The implementation generates all primitive long legs L = max(m²−n²,2mn),
   stores each L as an infinite sequence L,2L,3L,… and merges these
   sequences with a min‑heap to produce the sorted list.  The result is
   cached for repeated calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------- helper: GCD for small ints */
static int gcd_int(int a, int b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------------------------------------------------- heap entry */
typedef struct {
    seq_t val;   /* current multiple = base * k   */
    seq_t base;  /* primitive long leg          */
    idx_t k;     /* multiplier (>=1)             */
} HeapEntry;

/* dynamic heap storage */
static HeapEntry *heap_arr = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

/* dynamic cache storage */
static seq_t *cache_arr = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* forward declarations */
static void heap_push(HeapEntry e);
static HeapEntry heap_pop(void);
static void ensure_cache(idx_t need);
static void init_primitives(void);

/* --------------------------------------------------- heap utilities */

static void heap_increase_capacity(void)
{
    size_t new_cap = (heap_cap == 0) ? 1024 : heap_cap * 2;
    HeapEntry *tmp = (HeapEntry*)realloc(heap_arr, new_cap * sizeof(HeapEntry));
    heap_arr = tmp;
    heap_cap = new_cap;
}

static void heap_push(HeapEntry e)
{
    if (heap_size == heap_cap) heap_increase_capacity();

    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap_arr[parent].val <= e.val) break;
        heap_arr[i] = heap_arr[parent];
        i = parent;
    }
    heap_arr[i] = e;
}

static HeapEntry heap_pop(void)
{
    HeapEntry root = heap_arr[0];
    size_t last = --heap_size;
    HeapEntry e = heap_arr[last];

    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = (right < heap_size && heap_arr[right].val < heap_arr[left].val)
                       ? right : left;
        if (e.val <= heap_arr[child].val) break;
        heap_arr[i] = heap_arr[child];
        i = child;
    }
    heap_arr[i] = e;
    return root;
}

/* --------------------------------------------------- cache utilities */

static void cache_increase_capacity(void)
{
    size_t new_cap = (cache_cap == 0) ? 1024 : cache_cap * 2;
    seq_t *tmp = (seq_t*)realloc(cache_arr, new_cap * sizeof(seq_t));
    cache_arr = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------- generate primitives */

static void init_primitives(void)
{
    const int m_max = 500;               /* sufficient for n <= 10000     */
    for (int m = 2; m <= m_max; ++m) {
        for (int n = 1; n < m; ++n) {
            if ( ((m - n) & 1) && gcd_int(m, n) == 1 ) {
                seq_t a = (seq_t)m * (seq_t)m - (seq_t)n * (seq_t)n;
                seq_t b = (seq_t)2 * (seq_t)m * (seq_t)n;
                seq_t long_leg = a > b ? a : b;
                HeapEntry e = { long_leg, long_leg, 1 };
                heap_push(e);
            }
        }
    }
}

/* --------------------------------------------------- ensure cache has at least 'need' values */

static void ensure_cache(idx_t need)
{
    if (need == 0) return;
    if (heap_arr == NULL) init_primitives();

    while (cache_size < (size_t)need) {
        if (heap_size == 0) {
            /* Should never happen for the supported range */
            break;
        }

        HeapEntry e = heap_pop();
        if (cache_size == cache_cap) cache_increase_capacity();
        cache_arr[cache_size++] = e.val;

        e.k += 1;
        e.val = e.base * (seq_t)e.k;
        heap_push(e);
    }
}

/* --------------------------------------------------- public function */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n<1, return 0 */
    ensure_cache(n);
    return cache_arr[(size_t)n - 1];
}
