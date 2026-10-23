/*
 * A183234  Ordering of the numbers in tree A183232; complement of A183233.
 * https://oeis.org/A183234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   compare_seq_t, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequencing of the tree defined by
   2∈S, and if n∈S then (n²+5n+2)/2 and n+⌊½+√(2n+4)⌋ are in S.
   The function returns the nth smallest distinct element of S
   (indices start at 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/compare_seq_t.h"

/*---------------------------------------------------------------*/
/* Dynamic arrays for the sequence and a min‑heap of candidates   */
/*---------------------------------------------------------------*/
static seq_t *S = NULL;      /* sorted unique elements of S */
static size_t S_cap = 0, S_len = 0;
static seq_t *heap = NULL;   /* binary min‑heap of candidate values */
static size_t heap_cap = 0, heap_len = 0;

/* Ensure the sequence array has room for another value. */
static void ensure_S_cap(void) {
    if (S_len < S_cap) return;
    size_t new_cap = S_cap ? S_cap * 2 : 256;
#ifdef __cplusplus
    seq_t *tmp = (seq_t*)realloc(S, new_cap * sizeof(seq_t));
#else
    seq_t *tmp = realloc(S, new_cap * sizeof(seq_t));
#endif
    if (!tmp) exit(1);
    S = tmp;
    S_cap = new_cap;
}

/* Ensure the heap array has room for another value. */
static void ensure_heap_cap(void) {
    if (heap_len < heap_cap) return;
    size_t new_cap = heap_cap ? heap_cap * 2 : 256;
#ifdef __cplusplus
    seq_t *tmp = (seq_t*)realloc(heap, new_cap * sizeof(seq_t));
#else
    seq_t *tmp = realloc(heap, new_cap * sizeof(seq_t));
#endif
    if (!tmp) exit(1);
    heap = tmp;
    heap_cap = new_cap;
}

/*---------------------------------------------------------------*/
/* Min‑heap operations                                            */
/*---------------------------------------------------------------*/
static void heap_push(seq_t val) {
    ensure_heap_cap();
    size_t i = heap_len++;
    heap[i] = val;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (compare_seq_t(&heap[i], &heap[p]) < 0) {
            seq_t tmp = heap[i];
            heap[i] = heap[p];
            heap[p] = tmp;
            i = p;
        } else break;
    }
}

static seq_t heap_pop_min(void) {
    seq_t res = heap[0];
    heap[0] = heap[--heap_len];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_len && compare_seq_t(&heap[l], &heap[smallest]) < 0)
            smallest = l;
        if (r < heap_len && compare_seq_t(&heap[r], &heap[smallest]) < 0)
            smallest = r;
        if (smallest == i) break;
        seq_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/*---------------------------------------------------------------*/
/* Main sequence generator                                       */
/*---------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Initialise data structures on first call. */
    if (!S) {
        S = (seq_t*)malloc(256 * sizeof(seq_t));
        S_cap = 256;
        heap = (seq_t*)malloc(256 * sizeof(seq_t));
        heap_cap = 256;
        heap_push((seq_t)2);          /* 2 is the root of the tree */
    }

    /* Grow the sequence until we have at least n elements. */
    while (S_len < n) {
        if (heap_len == 0) break;   /* safety guard */

        seq_t x = heap_pop_min();

        /* Skip duplicates that may appear in the heap. */
        if (S_len > 0 && compare_seq_t(&x, &S[S_len - 1]) == 0)
            continue;

        /* Store the unique element. */
        ensure_S_cap();
        S[S_len++] = x;

        /* Child 1: (x² + 5x + 2) / 2 */
        seq_t child1 = (x * x + (seq_t)5 * x + (seq_t)2) / (seq_t)2;
        heap_push(child1);

        /* Child 2: x + floor(½ + sqrt(2x + 4)) */
        seq_t k = ((seq_t)2 * x) + (seq_t)4;          /* k = 2x + 4 */
        seq_t s = floor_sqrt(k);                     /* floor(sqrt(k)) */
        /* Determine whether sqrt(k) >= s + 0.5 */
        bool add_half = (k >= (s * s + s + (seq_t)1));
        seq_t add = s + (add_half ? (seq_t)1 : (seq_t)0);
        seq_t child2 = x + add;
        heap_push(child2);
    }

    return S[n - 1];
}
