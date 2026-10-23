/*
 * A009331  If a, b in sequence, so is ab+8.
 * https://oeis.org/A009331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009331
   a(1) = 2.
   If a,b are in the sequence, then ab+8 is also in the sequence.
   The function returns the n‑th term (n ≥ 1).  The sequence is
   generated lazily and cached, so calls with arbitrary order of n
   are allowed. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t val;      /* candidate value a*b+8 */
    size_t i;       /* index of the first factor a = seq[i] */
} HeapNode;

static seq_t *seq       = NULL;   /* generated terms */
static size_t seq_len   = 0;       /* current number of terms */
static size_t seq_cap   = 0;       /* allocated size of seq */

static size_t *j_idx    = NULL;   /* second‑factor index for each i */
static size_t j_cap     = 0;       /* allocated size of j_idx */

static HeapNode *heap   = NULL;   /* min‑heap of candidates */
static size_t heap_size = 0;      /* current heap occupancy */
static size_t heap_cap  = 0;      /* allocated size of heap */

static seq_t last_val   = (seq_t)0; /* last value inserted into seq */

/* ------------------------------------------------------------------ */
/* Helper: push a new candidate onto the heap (min‑heap invariant). */
static void heap_push(seq_t val, size_t i)
{
    if (heap_size >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 16;
        heap = (HeapNode*)realloc(heap, new_cap * sizeof(HeapNode));
        heap_cap = new_cap;
    }
    HeapNode node = { val, i };
    heap[heap_size++] = node;

    /* sift up */
    size_t idx = heap_size - 1;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= heap[idx].val)
            break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

/* Helper: remove and return the minimum element from the heap. */
static HeapNode heap_pop_min(void)
{
    HeapNode res = heap[0];
    heap[0] = heap[--heap_size];
    /* sift down */
    size_t idx = 0;
    while (true) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_size)
            break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[smallest].val >= heap[idx].val)
            break;
        HeapNode tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx] = tmp;
        idx = smallest;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Initialise the first term and the first candidate. */
static void init_sequence(void)
{
    /* allocate initial storage */
    seq_cap = 16;
    seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    j_cap  = 16;
    j_idx  = (size_t*)malloc(j_cap * sizeof(size_t));

    seq[0] = (seq_t)2;
    seq_len = 1;
    last_val = seq[0];

    j_idx[0] = 0;           /* j[0] = 0 for the first element itself */
    heap_push(seq[0] * seq[0] + (seq_t)8, 0);
}

/* ------------------------------------------------------------------ */
/* Ensure that at least 'target' terms are generated. */
static void ensure_n(size_t target)
{
    while (seq_len < target) {
        /* Extract the smallest candidate */
        HeapNode node = heap_pop_min();
        seq_t val = node.val;
        size_t i   = node.i;

        /* Append if this is a new distinct value */
        if (val != last_val) {
            /* grow seq array if necessary */
            if (seq_len + 1 > seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len] = val;
            seq_len++;

            /* grow j_idx array if necessary */
            if (seq_len > j_cap) {
                size_t new_cap = j_cap ? j_cap << 1 : 16;
                j_idx = (size_t*)realloc(j_idx, new_cap * sizeof(size_t));
                j_cap = new_cap;
            }
            j_idx[seq_len - 1] = seq_len - 1;   /* start with itself */

            /* push criterion for the new element: a*a+8 */
            heap_push(seq[seq_len - 1] * seq[seq_len - 1] + (seq_t)8,
                      seq_len - 1);

            last_val = val;
        }

        /* Advance the j pointer for the used i and push next candidate if possible */
        j_idx[i]++;
        if (j_idx[i] < seq_len) {
            seq_t new_val = seq[i] * seq[j_idx[i]] + (seq_t)8;
            heap_push(new_val, i);
        }
    }
}

/* ------------------------------------------------------------------ */
/* The sequence function required by the harness. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n = 0, but return 0 as sentinel */

    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    /* n is unsigned 64‑bit; cast to size_t is safe for the range used in tests */
    size_t target = (size_t)n;
    ensure_n(target);
    return seq[target - 1];
}
