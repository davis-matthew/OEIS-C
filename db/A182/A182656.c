/*
 * A182656  Floor-sum sequence of r, with r=sqrt(3), and a(1)=1, a(2)=2.
 * https://oeis.org/A182656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   ensure_nextJ_capacity, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Floor‑sum sequence of r = sqrt(3) with a(1) = 1, a(2) = 2
   (OEIS A182656).  Implements the incremental construction
   described in the OEIS entry. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_nextJ_capacity.h"

typedef struct { seq_t val; size_t i; } Node;

/* Ensure a sequence array has at least 'needed' capacity */
static void ensure_seq_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 64 : *cap * 2;
        while (new_cap < needed) new_cap *= 2;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Push a node onto a min‑heap ordered by .val then .i */
static void heap_push(Node **heap, size_t *cap, size_t *len, Node n)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 64 : *cap * 2;
        *heap = (Node *)realloc(*heap, new_cap * sizeof(Node));
        *cap = new_cap;
    }
    (*heap)[(*len)++] = n;

    size_t idx = *len - 1;
    while (idx > 0) {
        size_t p = (idx - 1) / 2;
        bool smaller = false;
        if ((*heap)[idx].val < (*heap)[p].val) smaller = true;
        else if ((*heap)[idx].val == (*heap)[p].val &&
                 (*heap)[idx].i < (*heap)[p].i)
            smaller = true;
        if (!smaller) break;
        Node tmp = (*heap)[idx];
        (*heap)[idx] = (*heap)[p];
        (*heap)[p] = tmp;
        idx = p;
    }
}

/* Pop the smallest node from the heap */
static Node heap_pop(Node **heap, size_t *len)
{
    Node res = (*heap)[0];
    (*heap)[0] = (*heap)[--(*len)];
    size_t idx = 0;
    while (1) {
        size_t l = idx * 2 + 1, r = l + 1, smallest = idx;
        if (l < *len &&
           ((*heap)[l].val < (*heap)[smallest].val ||
            ((*heap)[l].val == (*heap)[smallest].val &&
             (*heap)[l].i < (*heap)[smallest].i)))
            smallest = l;
        if (r < *len &&
           ((*heap)[r].val < (*heap)[smallest].val ||
            ((*heap)[r].val == (*heap)[smallest].val &&
             (*heap)[r].i < (*heap)[smallest].i)))
            smallest = r;
        if (smallest != idx) {
            Node tmp = (*heap)[idx];
            (*heap)[idx] = (*heap)[smallest];
            (*heap)[smallest] = tmp;
            idx = smallest;
        } else break;
    }
    return res;
}

/* Compute floor((a + b) * sqrt(3)) using integer arithmetic
   for any seq_t that supports +, *, and floor_sqrt. */
static inline seq_t combine_floor(seq_t a, seq_t b)
{
    seq_t s = a + b;
    seq_t prod = s * s;
    prod = prod * (seq_t)3;
    return floor_sqrt(prod);
}

/* Push a candidate for a fixed i onto the heap.
   It uses the current 'nextJ[i]' value to get j. */
static void push_candidate_for_i(size_t i,
                                 seq_t *S, size_t S_len,
                                 size_t *nextJ,
                                 Node **heap, size_t *heap_cap, size_t *heap_len)
{
    if (i >= S_len) return;
    size_t j = nextJ[i];
    if (j >= S_len) return;
    Node n;
    n.i = i;
    n.val = combine_floor(S[i], S[j]);
    heap_push(heap, heap_cap, heap_len, n);
}

/* Main sequence function as required by the OEIS tester. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *S = NULL;           /* sequence values            */
    static size_t S_len = 0, S_cap = 0;
    static size_t *nextJ = NULL;      /* next j>i to try            */
    static size_t nextJ_cap = 0;
    static Node *heap = NULL;         /* min‑heap of candidates     */
    static size_t heap_len = 0, heap_cap = 0;

    if (!initialized) {
        initialized = true;

        /* start sequence with the two seed values */
        ensure_seq_capacity(2, &S, &S_cap);
        S[0] = (seq_t)1;
        S[1] = (seq_t)2;
        S_len = 2;

        /* initialise nextJ: for i=0 j=1, for i=1 j=2 (invalid) */
        ensure_nextJ_capacity(2, &nextJ, &nextJ_cap);
        nextJ[0] = 1;
        nextJ[1] = 2;      /* invalid yet (>=S_len) */

        /* heap for candidates */
        heap = (Node *)malloc(64 * sizeof(Node));
        heap_cap = 64;
        heap_len = 0;

        /* push the first candidate (1,2) */
        push_candidate_for_i(0, S, S_len, nextJ, &heap, &heap_cap, &heap_len);
    }

    /* generate terms until we have at least n terms */
    while (S_len < (size_t)n) {
        /* pop smallest candidate from the heap */
        Node bk = heap_pop(&heap, &heap_len);
        size_t i = bk.i;
        size_t j = nextJ[i];
        seq_t val = bk.val;

        /* add new element if not a duplicate */
        bool added = false;
        if (S_len == 0 || val != S[S_len - 1]) {
            added = true;
            ensure_seq_capacity(S_len + 1, &S, &S_cap);
            S[S_len++] = val;

            /* allocate nextJ entry for the new index */
            ensure_nextJ_capacity(S_len, &nextJ, &nextJ_cap);
            nextJ[S_len - 1] = S_len;   /* no valid j yet (>=S_len) */
        }

        /* advance nextJ for i and push new candidate (if any) */
        nextJ[i]++;               /* move to next j > i */
        if (nextJ[i] < S_len)
            push_candidate_for_i(i, S, S_len, nextJ, &heap, &heap_cap, &heap_len);

        /* after a new addition, push candidate for predecessor */
        if (added && S_len >= 2)
            push_candidate_for_i(S_len - 2, S, S_len, nextJ, &heap, &heap_cap, &heap_len);
    }

    return S[(size_t)n - 1];
}
