/*
 * A125012  Numbers k for which nontrivial positive magic squares of exactly 5
 *          different orders with magic sum k exist. For a definition of
 *          nontrivial positive magic squares, see A125005.
 * https://oeis.org/A125012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th integer k for which there exist * exactly five
 *            different orders of a non‑trivial positive magic * square having
 *            magic sum k. * * The sequence is obtained by merging the
 *            arithmetic progressions * * S_n = M(n) + n·t , t ≥ 0, * * where
 *            M(n) = n·(n² + 1)/2 is the minimal possible sum...
 * Range:     u8 none, u16 none, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- heap node ------------------------------------------------ */
typedef struct {
    seq_t S;      /* current sum of the progression */
    idx_t step;   /* order n   (the step size in the progression) */
} Node;

/* ---------- heap implementation -------------------------------------- */
static Node *heap = NULL;          /* array of heap nodes */
static size_t heap_size = 0;       /* current number of nodes in heap */
static size_t heap_cap  = 0;       /* current capacity of the heap array */

/* grow the heap array when needed */
static void heap_ensure_cap(void)
{
    if (heap_size < heap_cap) return;
    size_t new_cap = heap_cap ? heap_cap << 1 : 16;
    heap = (Node *)realloc(heap, new_cap * sizeof(Node));
    heap_cap = new_cap;
}

/* Insert a node into the min‑heap */
static void heap_push(Node node)
{
    heap_ensure_cap();
    size_t i = heap_size++;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].S <= node.S) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i] = node;
}

/* Remove and return the smallest node from the heap */
static Node heap_pop(void)
{
    Node ret = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = (right < heap_size && heap[right].S < heap[left].S)
                       ? right : left;
        if (heap[child].S >= heap[i].S) break;
        Node tmp = heap[i];
        heap[i] = heap[child];
        heap[child] = tmp;
        i = child;
    }
    return ret;
}

/* Return the smallest node without removing it */
static Node heap_peek(void)
{
    return heap[0];
}

/* ---------- terms storage -------------------------------------------- */
static seq_t *terms = NULL;       /* array of found terms */
static size_t terms_cap = 0;      /* capacity of terms array */
static size_t terms_cnt = 0;      /* number of stored terms */

/* Ensure the terms array has room for at least one more element */
static void ensure_terms_cap(void)
{
    if (terms_cnt < terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 64;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* ---------- main function ------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* initialise the heap with orders 3 .. LIMIT */
        const idx_t LIMIT = 200000;     /* should be large enough for n <= 10 000 */
        heap_cap = 0;
        heap    = NULL;
        for (idx_t on = 3; on <= LIMIT; ++on) {
            seq_t n_sq = (seq_t)on * (seq_t)on;
            seq_t M    = ((n_sq + (seq_t)1) * (seq_t)on) / (seq_t)2;
            heap_push((Node){ M, on });
        }

        terms_cap = 0;
        terms    = NULL;
        terms_cnt= 0;

        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (terms_cnt < n) {
        Node cur = heap_pop();
        seq_t curS = cur.S;

        /* push the next sum of this progression */
        Node nxt = { curS + (seq_t)cur.step, cur.step };
        heap_push(nxt);

        /* process all nodes with the same sum (duplicate orders) */
        uint8_t cnt = 1;
        while (heap_size > 0 && heap_peek().S == curS) {
            Node dup = heap_pop();
            heap_push((Node){ dup.S + (seq_t)dup.step, dup.step });
            cnt++;
        }

        if (cnt == 5) {
            ensure_terms_cap();
            terms[terms_cnt++] = curS;
        }
    }

    return terms[n - 1];
}
