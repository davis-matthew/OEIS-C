/*
 * A072777  Powers of squarefree numbers that are not squarefree.
 * https://oeis.org/A072777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 229, u64 n <= 7949, u128 n <= 7949,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072777: powers of squarefree numbers that are not squarefree
   (OEIS, offset 1).  The sequence consists of all integers n = k^e
   with e ≥ 2, k ≥ 2 squarefree.  The numbers are listed in increasing
   order without duplicates. */

/* Implementation notes
   --------------------
   - Uses a min‑heap of nodes (value, base, exponent) to generate the
     sequence in ascending order.
   - All squarefree bases up to a fixed limit LIMIT are inserted into the
     heap initially.  LIMIT = 200 000 is large enough to provide
     more than 100 000 terms, which covers all requested indices up to
     10 000 comfortably.
   - The heap operations use the arithmetic type seq_t directly; the
     comparison operator < works both for the standard unsigned integer
     types and for the arbitrary‑precision class used in the C++ build.
   - The function a(idx_t n) is fully memoised: the terms are stored in
     a dynamic array that grows as needed.  Subsequent calls for
     larger n are O(1) per term.  The algorithm runs in O(n log SQFREE)
     time where SQFREE is the number of squarefree numbers ≤ LIMIT.

   - No external helper functions (e.g. seq_compare) are required.  The
     code is valid C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Parameters ---------------------------------------------------------- */
#define LIMIT 200000                 /* upper bound for squarefree bases */
static bool squarefree_table[LIMIT + 1];
static bool squarefree_table_ready = false;

/* --------------------------------------------------------------------- */
/* Heap implementation ------------------------------------------------- */
typedef struct {
    seq_t val;    /* current power value */
    seq_t base;   /* squarefree base */
    unsigned exp; /* current exponent (≥2) */
} node;

typedef struct {
    node *data;   /* array of nodes */
    size_t size;  /* number of elements */
    size_t capacity; /* allocated capacity */
} heap;

static heap pow_heap = { NULL, 0, 0 };

/* comparison of two nodes by value */
static int node_cmp(node *a, node *b)
{
    if (a->val < b->val) return -1;
    if (a->val > b->val) return 1;
    return 0;
}

static void heap_init(heap *h, size_t cap)
{
    h->data = (node *)malloc(cap * sizeof(node));
    h->size = 0;
    h->capacity = cap;
}

static void heap_free(heap *h)
{
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

static void heap_push(heap *h, node n)
{
    if (h->size == h->capacity) {
        h->capacity = h->capacity ? h->capacity * 2 : 16;
        h->data = (node *)realloc(h->data, h->capacity * sizeof(node));
    }
    size_t i = h->size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (node_cmp(&h->data[parent], &n) <= 0)
            break;
        h->data[i] = h->data[parent];
        i = parent;
    }
    h->data[i] = n;
}

static node heap_pop(heap *h)
{
    node top = h->data[0];
    node last = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= h->size)
            break;
        size_t right = left + 1;
        size_t child = (right < h->size &&
                        node_cmp(&h->data[right], &h->data[left]) < 0)
                       ? right : left;
        if (node_cmp(&last, &h->data[child]) <= 0)
            break;
        h->data[i] = h->data[child];
        i = child;
    }
    h->data[i] = last;
    return top;
}

/* --------------------------------------------------------------------- */
/* Utility: Build squarefree table ------------------------------------- */
static void ensure_squarefree_table(void)
{
    if (squarefree_table_ready)
        return;
    for (size_t i = 0; i <= LIMIT; ++i)
        squarefree_table[i] = true;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!squarefree_table[i])
            continue;
        size_t sq = i * i;
        for (size_t j = sq; j <= LIMIT; j += sq)
            squarefree_table[j] = false;
    }
    squarefree_table_ready = true;
}

/* --------------------------------------------------------------------- */
/* Initialise the heap with the squares of all squarefree bases -------- */
static void init_heap(void)
{
    if (pow_heap.data != NULL)
        return; /* already initialised */

    heap_init(&pow_heap, 0);
    ensure_squarefree_table();

    for (size_t a = 2; a <= LIMIT; ++a) {
        if (!squarefree_table[a])
            continue;
        node n;
        n.base = (seq_t)a;
        n.exp  = 2;
        n.val  = (seq_t)a * (seq_t)a;
        heap_push(&pow_heap, n);
    }
}

/* --------------------------------------------------------------------- */
/* Dynamic array of computed terms ------------------------------------ */
static seq_t *terms = NULL;
static size_t terms_capacity = 0;
static size_t terms_size = 0;

static void grow_terms(size_t needed)
{
    if (terms_capacity >= needed)
        return;
    while (terms_capacity < needed) {
        terms_capacity = terms_capacity ? terms_capacity * 2 : 64;
    }
    terms = (seq_t *)realloc(terms, terms_capacity * sizeof(seq_t));
}

static void ensure_terms(idx_t target)
{
    if (target <= (idx_t)terms_size)
        return;

    init_heap();
    grow_terms((size_t)target);

    while ((idx_t)terms_size < target) {
        node cur = heap_pop(&pow_heap);
        terms[terms_size++] = cur.val;

        node next;
        next.base = cur.base;
        next.exp  = cur.exp + 1;
        next.val  = cur.val * cur.base;  /* multiply by base */
        heap_push(&pow_heap, next);
    }
}

/* --------------------------------------------------------------------- */
/*  Public sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; undefined for n=0, harmless */
        return 0;
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
