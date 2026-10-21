/*
 * A057101  Area of a Pythagorean triangle (ordered by the product of the
 *          sides).
 * https://oeis.org/A057101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A057101 by merging primitive Pythagorean triples and their
 * multiples ordered by the product a·b·c. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* gcd for small integers */
static inline int gcd_int(int a, int b)
{
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* node of the min‑heap */
typedef struct {
    seq_t prod;  /* a·b·c */
    seq_t area;  /* a·b/2 */
    seq_t P0;    /* product of the base primitive */
    seq_t A0;    /* area of the base primitive */
    uint64_t k;  /* scaling factor */
} Node;

/* simple binary min‑heap (1‑based index) */
typedef struct {
    Node *data;
    size_t size;
    size_t capacity;
} Heap;

static Heap heap = {NULL, 0, 0};

static void heap_reserve(Heap *h, size_t min_cap)
{
    if (h->capacity >= min_cap) return;
    size_t new_cap = h->capacity ? h->capacity * 2 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    Node *new_data = (Node *)realloc(h->data, new_cap * sizeof(Node));
    h->data = new_data;
    h->capacity = new_cap;
}

static inline bool heap_less(const Node *a, const Node *b)
{
    return a->prod < b->prod;
}

static void heap_push(Heap *h, Node n)
{
    heap_reserve(h, h->size + 1);
    size_t i = ++h->size;
    while (i > 1) {
        size_t p = i >> 1;
        if (!heap_less(&n, &h->data[p])) break;
        h->data[i] = h->data[p];
        i = p;
    }
    h->data[i] = n;
}

static Node heap_pop(Heap *h)
{
    Node ret = h->data[1];
    Node last = h->data[h->size--];
    size_t i = 1;
    while (1) {
        size_t l = i << 1;
        if (l > h->size) break;
        size_t r = l + 1;
        size_t child = (r <= h->size && heap_less(&h->data[r], &h->data[l])) ? r : l;
        if (!heap_less(&h->data[child], &last)) break;
        h->data[i] = h->data[child];
        i = child;
    }
    h->data[i] = last;
    return ret;
}

/* storage for primitive triples */
#define MAX_M 500  /* enough to generate >10 000 terms */
static seq_t *P0_arr = NULL;   /* product of primitive triple */
static seq_t *A0_arr = NULL;   /* area of primitive triple */
static size_t prim_cnt = 0;
static size_t prim_cap = 0;

static void init_primitives(void)
{
    if (prim_cnt != 0) return;   /* already done */

    prim_cap = 256;
    P0_arr = (seq_t *)malloc(prim_cap * sizeof(seq_t));
    A0_arr = (seq_t *)malloc(prim_cap * sizeof(seq_t));

    for (int m = 2; m <= MAX_M; ++m) {
        for (int n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0) continue;   /* require opposite parity */
            if (gcd_int(m, n) != 1) continue;   /* coprime */

            seq_t mm = (seq_t)m;
            seq_t nn = (seq_t)n;
            seq_t a  = mm * mm - nn * nn;
            seq_t b  = 2 * mm * nn;
            seq_t c  = mm * mm + nn * nn;
            seq_t A0 = (a * b) / 2;
            seq_t P0 = a * b * c;

            if (prim_cnt == prim_cap) {
                prim_cap <<= 1;
                P0_arr = (seq_t *)realloc(P0_arr, prim_cap * sizeof(seq_t));
                A0_arr = (seq_t *)realloc(A0_arr, prim_cap * sizeof(seq_t));
            }
            P0_arr[prim_cnt] = P0;
            A0_arr[prim_cnt] = A0;
            ++prim_cnt;
        }
    }
}

static void init_heap(void)
{
    if (heap.size != 0) return;   /* already done */

    heap_reserve(&heap, prim_cnt);
    for (size_t i = 0; i < prim_cnt; ++i) {
        Node n;
        n.prod = P0_arr[i];
        n.area = A0_arr[i];
        n.P0   = P0_arr[i];
        n.A0   = A0_arr[i];
        n.k    = 1;
        heap_push(&heap, n);
    }
}

/* storage for computed areas */
static seq_t *area_arr = NULL;
static size_t area_alloc = 0;
static size_t area_count = 0;

static void area_reserve(size_t min_cap)
{
    if (area_alloc >= min_cap) return;
    size_t new_cap = area_alloc ? area_alloc * 2 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    area_arr = (seq_t *)realloc(area_arr, new_cap * sizeof(seq_t));
    area_alloc = new_cap;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_primitives();
        init_heap();
        initialized = true;
    }

    if (area_count < n) {
        area_reserve(n);
        while (area_count < n) {
            Node cur = heap_pop(&heap);

            /* store area of the popped triple/multiple */
            area_arr[area_count++] = cur.area;

            /* generate next multiple of the same primitive */
            uint64_t next_k = cur.k + 1;
            seq_t kk   = (seq_t)next_k;
            seq_t kk2  = kk * kk;
            seq_t kk3  = kk2 * kk;

            Node next;
            next.P0   = cur.P0;
            next.A0   = cur.A0;
            next.prod = cur.P0 * kk3; /* (a·b·c) for the next multiple */
            next.area = cur.A0 * kk2; /* (a·b/2) for the next multiple */
            next.k    = next_k;
            heap_push(&heap, next);
        }
    }
    return area_arr[n - 1];
}
