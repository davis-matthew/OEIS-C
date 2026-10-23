/*
 * A184411  Ranks of (even i)+j/r when all i+j/r are ranked; r=sqrt(2), i>=0,
 *          j>=0. Complement of A184410.
 * https://oeis.org/A184411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A184411 – Ranks of (even i)+j/√2 when all i+j/√2 are ranked.
 *
 *  Formula: a(n) = the rank of the nth lattice point (i,j) with i even
 *  in the ordering of i + j/√2 (r = √2).  The implementation uses a
 *  min‑heap to generate points in increasing order and records the
 *  ranks of points with even i.  It works for any seq_t and is
 *  incremental, so repeated calls with arbitrary n are fast.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define HASH_CAP 65536U                      /* power‑of‑two hash size */
#define SQRT2 1.414213562373095048801688724209698079L

/*------------------------------------------------------------
 *  Heap node: a lattice point and its value i + j/√2
 *------------------------------------------------------------*/
typedef struct {
    idx_t i;
    idx_t j;
    long double v;   /* for ordering only */
} Node;

/*------------------------------------------------------------
 *  Simple binary min‑heap of nodes
 *------------------------------------------------------------*/
typedef struct {
    Node *data;
    size_t sz;
    size_t cap;
} Heap;

static void heap_init(Heap *h)
{
    h->cap = 256;
    h->sz  = 0;
    h->data = (Node *)malloc(h->cap * sizeof(Node));
}

static void heap_grow(Heap *h)
{
    h->cap <<= 1;
    h->data = (Node *)realloc(h->data, h->cap * sizeof(Node));
}

static void heap_swap(Node *a, Node *b)
{
    Node t = *a; *a = *b; *b = t;
}

static void heap_push(Heap *h, Node n)
{
    if (h->sz >= h->cap) heap_grow(h);
    size_t p = h->sz++;
    h->data[p] = n;
    while (p > 0) {
        size_t parent = (p - 1) >> 1;
        if (h->data[parent].v <= h->data[p].v) break;
        heap_swap(&h->data[parent], &h->data[p]);
        p = parent;
    }
}

static Node heap_pop(Heap *h)
{
    Node ret = h->data[0];
    h->data[0] = h->data[--h->sz];
    size_t p = 0;
    while (true) {
        size_t left  = (p << 1) + 1;
        size_t right = left + 1;
        size_t best  = p;
        if (left  < h->sz && h->data[left].v  < h->data[best].v) best = left;
        if (right < h->sz && h->data[right].v < h->data[best].v) best = right;
        if (best == p) break;
        heap_swap(&h->data[p], &h->data[best]);
        p = best;
    }
    return ret;
}

/*------------------------------------------------------------
 *  Hash set for 64‑bit keys (i<<32 | j)
 *------------------------------------------------------------*/
static uint64_t *hash_tbl = NULL;

static inline size_t hash_index(uint64_t key)
{
    return (key * 11400714819323198485ull) & (HASH_CAP - 1);
}

static bool hash_contains(uint64_t key)
{
    key += 1ull;                         /* 0 reserved for empty */
    size_t idx = hash_index(key);
    while (hash_tbl[idx]) {
        if (hash_tbl[idx] == key) return true;
        idx = (idx + 1) & (HASH_CAP - 1);
    }
    return false;
}

static void hash_insert(uint64_t key)
{
    key += 1ull;
    size_t idx = hash_index(key);
    while (hash_tbl[idx]) idx = (idx + 1) & (HASH_CAP - 1);
    hash_tbl[idx] = key;
}

/*------------------------------------------------------------
 *  State for incremental generation
 *------------------------------------------------------------*/
static bool   initialized = false;
static Heap   heap;
static seq_t *a_tab   = NULL;
static size_t a_cap   = 0;
static size_t a_used  = 0;
static idx_t  global_rank = 0;   /* number of points generated */

/*------------------------------------------------------------
 *  Insert new point into heap if unseen
 *------------------------------------------------------------*/
static void push_point(idx_t i, idx_t j)
{
    uint64_t key = ((uint64_t)i << 32) | (uint64_t)j;
    if (hash_contains(key)) return;
    hash_insert(key);
    Node n = { i, j, (long double)i + (long double)j / SQRT2 };
    heap_push(&heap, n);
}

/*------------------------------------------------------------
 *  Main function – A184411
 *------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */

    if (!initialized) {
        /* initialise structures */
        heap_init(&heap);
        hash_tbl = (uint64_t *)calloc(HASH_CAP, sizeof(uint64_t));

        a_cap = 256;
        a_tab = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_used = 0;
        global_rank = 0;

        push_point(0, 0);          /* seed with (0,0) */
        initialized = true;
    }

    /* produce even‑i ranks until we have at least n */
    while (a_used < (size_t)n) {
        Node cur = heap_pop(&heap);
        ++global_rank;

        if ((cur.i & 1ULL) == 0ULL) {   /* even i */
            if (a_used == a_cap) {
                a_cap <<= 1;
                a_tab = (seq_t *)realloc(a_tab, a_cap * sizeof(seq_t));
            }
            a_tab[a_used++] = (seq_t)global_rank;
        }

        /* enqueue neighbours */
        push_point(cur.i + 1, cur.j);
        push_point(cur.i, cur.j + 1);
    }

    return a_tab[n - 1];
}
