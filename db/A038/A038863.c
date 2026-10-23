/*
 * A038863  Numbers ending with '8' that are the difference of two positive
 *          cubes.
 * https://oeis.org/A038863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A038863
 *  a(n) = smallest number ending with 8 that can be expressed as x^3 - y^3
 *   with x > y > 0, listed in increasing order.
 *
 *  The code builds the sequence incrementally using a min‑heap over the
 *  already seen differences for each fixed lower cube y.  It memoises all
 *  computed terms so that subsequent calls with any n are answered in O(1).
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------- helper  -------------------- */

static seq_t cube(idx_t x)
{
    seq_t r = (seq_t)x;
    return r * r * r;
}

/* ---------- min‑heap of (difference, y, next_x) -------------- */
#define A_LIMIT 20000                 /* number of y values pre‑seeded */

typedef struct {
    seq_t val;     /* x^3 - y^3 */
    idx_t y;       /* lower cube index (y)      */
    idx_t x;       /* current upper cube index  */
} HeapNode;

static HeapNode heap[A_LIMIT + 4];
static int heap_size = 0;

static void heap_push(HeapNode node)
{
    int i = heap_size;
    heap[heap_size++] = node;
    while (i > 0) {
        int parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static HeapNode heap_pop(void)
{
    HeapNode ret = heap[0];
    --heap_size;
    heap[0] = heap[heap_size];
    int i = 0;
    while (1) {
        int left = (i << 1) + 1;
        if (left >= heap_size) break;
        int right = left + 1;
        int child = left;
        if (right < heap_size && heap[right].val < heap[left].val)
            child = right;
        if (heap[i].val <= heap[child].val) break;
        HeapNode tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }
    return ret;
}

/* ---------- dynamic array for sequence terms ---------------- */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

static void ensure_terms_cap(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* fallback */
    terms = tmp;
    terms_cap = new_cap;
}

/* ----------------- main function --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 */

    static bool initialized = false;

    if (!initialized) {
        /* initialise heap with the first difference for each y=1..A_LIMIT */
        for (idx_t y = 1; y <= A_LIMIT; ++y) {
            HeapNode node;
            node.y = y;
            node.x = y + 1;
            node.val = cube(node.x) - cube(node.y);
            heap_push(node);
        }
        initialized = true;
    }

    /* extend sequence until we have at least n terms */
    while (terms_len < n) {
        HeapNode cur = heap_pop();

        /* store if ends with digit 8 and not a duplicate */
        if ((cur.val % (seq_t)10) == (seq_t)8) {
            if (terms_len == 0 || cur.val != terms[terms_len - 1]) {
                ensure_terms_cap(terms_len + 1);
                terms[terms_len++] = cur.val;
            }
        }

        /* advance this y to the next x */
        ++cur.x;
        cur.val = cube(cur.x) - cube(cur.y);
        heap_push(cur);
    }

    return terms[n - 1];
}
