/*
 * A167971  Signature sequence of Phi^3 = 4.2360679774998..., where Phi is the
 *          golden ratio 1.6180339887499... .
 * https://oeis.org/A167971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Signature sequence of φ³ = 4.236067977…  
   Computed by incremental scheduling: after the k‑th occurrence of
   integer m, the next occurrence is at position pos + ⌊k·φ³⌋ + m,
   where φ³ = 2 + √5. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*----- Integer square root of a 64‑bit value -----*/
static inline idx_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/*----- Floor of k·φ³, where φ³ = 2 + √5 -----*/
static inline idx_t floor_phi3(idx_t k)
{
    /* φ³ = 2 + √5  →  ⌊k·φ³⌋ = 2k + ⌊k·√5⌋ */
    idx_t sq = int_sqrt_u64(5 * k * k);   /* ⌊k·√5⌋ */
    return 2 * k + sq;
}

/*----- Heap node (scheduled position, integer) -----*/
typedef struct { idx_t pos; idx_t num; } heap_node;

/*----- Global state for the generator -----*/
static heap_node *heap      = NULL;
static size_t    heap_cap   = 0;
static size_t    heap_size  = 0;

static idx_t *occ        = NULL;
static size_t  occ_cap   = 0;

/* Sequence cache */
static seq_t *seq_cache  = NULL;
static size_t seq_cap    = 0;
static size_t seq_len    = 0;

static idx_t next_unused = 1;

/*----- Heap helpers -----*/
static void heap_grow(void)
{
    size_t new_cap = (heap_cap == 0) ? 16 : heap_cap << 1;
    heap_node *new_arr = (heap_node *)realloc(heap, new_cap * sizeof(heap_node));
    heap = new_arr;
    heap_cap = new_cap;
}

static void heap_push(idx_t pos, idx_t num)
{
    if (heap_size == heap_cap) heap_grow();
    size_t i = heap_size++;
    heap[i].pos = pos;
    heap[i].num = num;
    /* sift up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].pos < heap[i].pos ||
           (heap[parent].pos == heap[i].pos && heap[parent].num <= heap[i].num))
            break;
        heap_node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static void heap_pop(heap_node *out)
{
    *out = heap[0];
    heap[0] = heap[--heap_size];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap_size) {
            if (heap[left].pos < heap[smallest].pos ||
               (heap[left].pos == heap[smallest].pos && heap[left].num < heap[smallest].num))
                smallest = left;
        }
        if (right < heap_size) {
            if (heap[right].pos < heap[smallest].pos ||
               (heap[right].pos == heap[smallest].pos && heap[right].num < heap[smallest].num))
                smallest = right;
        }
        if (smallest == i) break;
        heap_node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/*----- Sequence generator -----*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0 */

    /* Expand sequence cache if needed */
    while (seq_len < n) {
        idx_t pos = seq_len + 1;          /* next position (1‑based) */
        idx_t m;

        /* Decide numeric value at this position */
        if (heap_size > 0 && heap[0].pos == pos) {
            heap_node node;
            heap_pop(&node);
            m = node.num;
        } else {
            /* No scheduled value => introduce a new integer */
            m = next_unused++;
            if (m >= occ_cap) {
                size_t new_cap = occ_cap ? occ_cap << 1 : 16;
                if (new_cap < m + 1) new_cap = m + 1;
                idx_t *new_arr = (idx_t *)realloc(occ, new_cap * sizeof(idx_t));
                occ = new_arr;
                occ_cap = new_cap;
            }
            occ[m] = 0;
        }

        /* Ensure cache capacity */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
            seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = new_arr;
            seq_cap = new_cap;
        }

        /* Record value */
        seq_cache[seq_len++] = (seq_t)m;

        /* Update occurrence counter and schedule next occurrence */
        occ[m]++;                        /* now k = occ[m] */
        idx_t k = occ[m];
        idx_t inc = floor_phi3(k) + m;   /* distance to next occurrence */
        idx_t next_pos = pos + inc;
        heap_push(next_pos, m);
    }

    return seq_cache[n - 1];
}
