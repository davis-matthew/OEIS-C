/*
 * A140332  Products of two palindromes in base 10.
 * https://oeis.org/A140332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  a(n) = the n‑th distinct product of two decimal palindromes
 * (in increasing order, 0 is the first term, offset 1).
 *
 * Implementation details:
 *   • Generates all base‑10 palindromes up to 6 digits (0 … 999,999).
 *   • Uses a binary heap of index pairs (i , j) to enumerate all products
 *     in sorted order without storing the product values in the heap.
 *   • Stores the sequence itself in a dynamic array of seq_t.
 *     The array is grown only in powers of two so that realloc never
 *     invalidates existing pointers to seq_t objects.
 *   • Only trivial types are stored in the heap, thus realloc is safe
 *     even if seq_t is a non‑trivial arbitrary‑precision class.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- Palindrome generator (0 … 999,999) ----------------------- */
static seq_t *pal = NULL;      /* array of palindromes (sorted ascending)    */
static idx_t  pal_cnt = 0;     /* number of palindromes                      */

/* Maximum number of palindromes <= 999999 (1998); a slight over‑estimate. */
#define PAL_ESTIMATE 2000

static void generate_palindromes(void)
{
    const seq_t LIMIT = 999999;
    pal = (seq_t *)malloc(PAL_ESTIMATE * sizeof(seq_t));
    if (!pal) return;               /* allocation failure -> empty list */

    idx_t c = 0;
    pal[c++] = 0;

    /* 1‑digit palindromes */
    for (seq_t d = 1; d <= 9; ++d)
        pal[c++] = d;

    /* 2‑digit */
    for (seq_t d = 1; d <= 9; ++d)
        pal[c++] = d * 10 + d;

    /* 3‑digit */
    for (seq_t d = 1; d <= 9; ++d)
        for (seq_t m = 0; m <= 9; ++m)
            pal[c++] = d * 100 + m * 10 + d;

    /* 4‑digit */
    for (seq_t d = 1; d <= 9; ++d)
        for (seq_t m = 0; m <= 9; ++m)
            pal[c++] = d * 1000 + m * 100 + m * 10 + d;

    /* 5‑digit */
    for (seq_t d = 1; d <= 9; ++d)
        for (seq_t m = 0; m <= 9; ++m)
            for (seq_t n = 0; n <= 9; ++n)
                pal[c++] = d * 10000 + m * 1000 + n * 100 + m * 10 + d;

    /* 6‑digit */
    for (seq_t d = 1; d <= 9; ++d)
        for (seq_t m = 0; m <= 9; ++m)
            for (seq_t n = 0; n <= 9; ++n)
                pal[c++] = d * 100000 + m * 10000 + n * 1000 + n * 100 + m * 10 + d;

    pal_cnt = c;                       /* store actual count */
}

/* ---------- Heap of index pairs ------------------------------------ */
typedef struct {
    idx_t i;          /* index of first palindrome */
    idx_t j;          /* index of second palindrome (j >= i) */
} HeapNode;

static HeapNode *heap = NULL;   /* binary heap */
static size_t heap_cnt = 0;
static size_t heap_cap = 0;

/* Ensure capacity for at least "new_cap" nodes */
static void heap_reserve(size_t new_cap)
{
    if (new_cap <= heap_cap) return;
    size_t old_cap = heap_cap;
    heap_cap = (new_cap > old_cap * 2) ? new_cap : old_cap * 2;
    heap = (HeapNode *)realloc(heap, heap_cap * sizeof(HeapNode));
}

/* Return product for a given heap node */
static inline seq_t product(HeapNode *n)
{
    return pal[n->i] * pal[n->j];
}

/* Heap push */
static void heap_push(HeapNode v)
{
    heap_reserve(heap_cnt + 1);
    size_t k = heap_cnt++;
    heap[k] = v;

    /* bubble up */
    while (k > 0) {
        size_t p = (k - 1) >> 1;
        if (product(&heap[p]) <= product(&heap[k])) break;
        HeapNode tmp = heap[p];
        heap[p] = heap[k];
        heap[k] = tmp;
        k = p;
    }
}

/* Heap pop (returns smallest node in *out) */
static void heap_pop(HeapNode *out)
{
    if (heap_cnt == 0) return;             /* should not happen */
    *out = heap[0];
    heap[0] = heap[--heap_cnt];

    /* bubble down */
    size_t k = 0;
    while (1) {
        size_t l = (k << 1) + 1;
        size_t r = l + 1;
        if (l >= heap_cnt) break;
        size_t m = l;
        if (r < heap_cnt && product(&heap[r]) < product(&heap[l]))
            m = r;
        if (product(&heap[k]) <= product(&heap[m])) break;
        HeapNode tmp = heap[k];
        heap[k] = heap[m];
        heap[m] = tmp;
        k = m;
    }
}

/* ---------- Sequence storage --------------------------------------- */
static seq_t *seq = NULL;   /* array of unique products (sorted) */
static size_t seq_cnt = 0;
static size_t seq_cap = 0;

static void seq_reserve(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t old_cap = seq_cap;
    seq_cap = (new_cap > old_cap * 2) ? new_cap : old_cap * 2;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
}

static void seq_push(seq_t val)
{
    seq_reserve(seq_cnt + 1);
    seq[seq_cnt++] = val;
}

/* ---------- Initialization ---------------------------------------- */
static bool initialized = false;

static void init_sequence(void)
{
    generate_palindromes();

    /* each palindrome multiplied by itself is the first entry in its row */
    for (idx_t i = 0; i < pal_cnt; ++i) {
        HeapNode n;
        n.i = i; n.j = i;
        heap_push(n);
    }
    initialized = true;
}

/* ---------- Public function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, guard */

    if (!initialized) init_sequence();

    while (seq_cnt < n) {
        HeapNode cur;
        heap_pop(&cur);
        seq_t cur_val = product(&cur);

        /* store only new (distinct) values */
        if (seq_cnt == 0 || cur_val != seq[seq_cnt - 1])
            seq_push(cur_val);

        /* enqueue next element in the same row, if any */
        if (cur.j + 1 < pal_cnt) {
            HeapNode nxt;
            nxt.i = cur.i;
            nxt.j = cur.j + 1;
            heap_push(nxt);
        }
    }
    return seq[n - 1];
}
