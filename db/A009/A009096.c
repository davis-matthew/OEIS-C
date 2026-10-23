/*
 * A009096  Ordered perimeters of Pythagorean triangles, listed with
 *          multiplicity.
 * https://oeis.org/A009096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest value in the multiset of all perimeters
 *            of Pythagorean triples, listed in ascending order and with
 *            multiplicity. The perimeters are generated as all multiples of
 *            primitive perimeters 2m(m+n) for coprime integers m>n of
 *            opposite parity
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Helper for computing gcd of two unsigned integers.
   ------------------------------------------------------------------ */
static unsigned long long gcd_idx(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------
   Structure for heap items: current perimeter value and its increment.
   ------------------------------------------------------------------ */
typedef struct {
    seq_t val;   /* current perimeter to output */
    seq_t inc;   /* step: primitive perimeter value */
} heap_item_t;

/* ------------------------------------------------------------------
   Static storage used by function a: initialization, heap, and result
   array.  The initialization is performed on the first call.
   ------------------------------------------------------------------ */
static heap_item_t *heap = NULL;
static size_t heap_size = 0;

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static int initialized = 0;

/* ------------------------------------------------------------------
   Heap helper functions
   ------------------------------------------------------------------ */
static void sift_down(size_t i)
{
    size_t size = heap_size;
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < size && heap[right].val < heap[smallest].val)
            smallest = right;

        if (smallest == i)
            break;

        heap_item_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* ------------------------------------------------------------------
   Build min-heap from an arbitrary array
   ------------------------------------------------------------------ */
static void heap_build(void)
{
    for (size_t i = (heap_size >> 1); i-- > 0; )
        sift_down(i);
}

/* ------------------------------------------------------------------
   Initialize static data structures.
   ------------------------------------------------------------------ */
static void init(void)
{
    /* ------------------------------------------------------------------
       1. Generate primitive perimeters p0 = 2*m*(m+n) up to a bound.
          The bound is chosen large enough to guarantee at least 10 000
          perimeters after assembling multiples.
       ------------------------------------------------------------------ */
    const seq_t PERIM_LIMIT = 200000u;   /* safe upper bound */

    size_t prim_cap = 128;
    seq_t *prim = (seq_t *)malloc(prim_cap * sizeof(seq_t));
    size_t prim_cnt = 0;

    for (unsigned long long m = 2;
         2ULL * m * (m + 1ULL) <= PERIM_LIMIT;
         ++m)
    {
        for (unsigned long long n = 1; n < m; ++n) {
            if (((m - n) & 1ULL) == 0ULL)          /* same parity -> skip */
                continue;
            if (gcd_idx(m, n) != 1ULL)             /* not coprime -> skip */
                continue;

            seq_t p0 = 2ULL * (seq_t)m * (seq_t)(m + n);
            if (p0 > PERIM_LIMIT)
                break;                            /* further n only increases */

            if (prim_cnt == prim_cap) {
                prim_cap <<= 1;
                prim = (seq_t *)realloc(prim, prim_cap * sizeof(seq_t));
            }
            prim[prim_cnt++] = p0;
        }
    }

    /* ------------------------------------------------------------------
       2. Build heap of one entry for each primitive perimeter.
          Each entry stores current value (initially p0) and increment
          equal to p0.
       ------------------------------------------------------------------ */
    heap_size = prim_cnt;
    heap = (heap_item_t *)malloc(heap_size * sizeof(heap_item_t));
    for (size_t i = 0; i < prim_cnt; ++i) {
        heap[i].val = prim[i];
        heap[i].inc = prim[i];
    }
    heap_build();   /* turn array into min-heap */

    free(prim);     /* primitive list no longer needed */

    /* ------------------------------------------------------------------
       3. Prepare sequence result array.
       ------------------------------------------------------------------ */
    seq_cap = 128;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    initialized = 1;
}

/* ------------------------------------------------------------------
   The required function: returns the n-th term (1-indexed).
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for 0, return 0 for safety */

    if (!initialized)
        init();

    if ((size_t)n <= seq_len)
        return seq[n - 1];

    while (seq_len < n) {
        seq_t out_val = heap[0].val;          /* smallest available */

        /* grow result array if necessary */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 128;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }

        seq[seq_len++] = out_val;            /* store output */

        heap[0].val += heap[0].inc;          /* next multiple of same primitive */
        sift_down(0);                        /* restore heap property */
    }

    return seq[n - 1];
}
