/*
 * A057100  Hypotenuses of Pythagorean triangles (ordered by the product of
 *          the sides).
 * https://oeis.org/A057100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A057100:
   hypotenuses of Pythagorean triangles ordered by the product of the three sides a*b*c.
   The algorithm generates all primitive triples via Euclid's formula, stores them in
   a min‑heap keyed by the product a*b*c multiplied by k^3 (k is the common factor).
   The heap is then used to extract the triples in ascending order of that product,
   yielding the required hypotenuses. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd64.h"

/* Euclid's algorithm for uint64_t – used for primitive generation. */

/* ------------------------------------------------------------------ */
/* Data structures                                                     */
/* ------------------------------------------------------------------ */
typedef struct {
    seq_t    baseProdFull;   /* product a*b*c of the primitive triple   */
    seq_t    baseC;          /* hypotenuse of the primitive triple      */
    uint32_t k;              /* current multiplier (k ≥ 1)                */
} HeapEntry;

static HeapEntry *heap      = NULL;
static size_t    heap_size = 0;
static size_t    heap_cap  = 0;

static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* ------------------------------------------------------------------ */
/* Helper functions                                                    */
/* ------------------------------------------------------------------ */
static inline seq_t key_of(const HeapEntry *e) {
    /* key = baseProdFull * k^3  (computed in seq_t) */
    seq_t key = e->baseProdFull;
    key = key * (seq_t)e->k;
    key = key * (seq_t)e->k;
    key = key * (seq_t)e->k;
    return key;
}

static inline void heap_swap(size_t i, size_t j) {
    HeapEntry tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

/* ------------------------------------------------------------------ */
/* Heap operations                                                     */
/* ------------------------------------------------------------------ */
static void heap_push(HeapEntry e) {
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 16;
        heap = (HeapEntry *)realloc(heap, heap_cap * sizeof(HeapEntry));
    }
    size_t i = heap_size++;
    heap[i] = e;
    /* sift‑up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (key_of(&heap[p]) <= key_of(&heap[i]))
            break;
        heap_swap(i, p);
        i = p;
    }
}

static HeapEntry heap_pop(void) {
    HeapEntry top = heap[0];
    heap[0] = heap[--heap_size];
    /* sift‑down */
    size_t i = 0;
    while (true) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && key_of(&heap[l]) < key_of(&heap[smallest]))
            smallest = l;
        if (r < heap_size && key_of(&heap[r]) < key_of(&heap[smallest]))
            smallest = r;
        if (smallest == i)
            break;
        heap_swap(i, smallest);
        i = smallest;
    }
    return top;
}

/* ------------------------------------------------------------------ */
/* Primitive triple generation                                        */
/* ------------------------------------------------------------------ */
static void generate_primitives(void) {
    /* A limit that yields more primitives than needed for 10 000 terms. */
    const uint64_t M_LIMIT = 2000;
    for (uint64_t m = 2; m <= M_LIMIT; ++m) {
        for (uint64_t n = 1; n < m; ++n) {
            if (((m + n) & 1) && gcd64(m, n) == 1) {
                uint64_t a = m * m - n * n;
                uint64_t b = 2 * m * n;
                uint64_t c = m * m + n * n;
                /* store a ≤ b for consistency, although not needed for product */
                if (a > b) {
                    uint64_t t = a; a = b; b = t;
                }
                HeapEntry e;
                e.baseProdFull = (seq_t)a * (seq_t)b * (seq_t)c;
                e.baseC        = (seq_t)c;
                e.k            = 1;
                heap_push(e);
            }
        }
    }
}

static inline void ensure_seq_cap(size_t need) {
    if (need > seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------ */
/* Public function                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but harmless */

    static bool initialized = false;
    if (!initialized) {
        heap_cap = 0;
        heap_size = 0;
        seq_cap = 0;
        seq_len = 0;
        heap = NULL;
        seq_arr = NULL;
        generate_primitives();
        initialized = true;
    }

    while (seq_len < n) {
        HeapEntry e = heap_pop();

        seq_t hyp = e.baseC * (seq_t)e.k;
        ensure_seq_cap(seq_len + 1);
        seq_arr[seq_len++] = hyp;

        /* push the next multiple of this primitive triple */
        HeapEntry next = e;
        ++next.k;
        heap_push(next);
    }

    return seq_arr[n - 1];
}
