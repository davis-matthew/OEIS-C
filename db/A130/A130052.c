/*
 * A130052  Numbers that are the sum of one or more consecutive squares in
 *          more than one way.
 * https://oeis.org/A130052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 8920, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A130052 – Numbers that are the sum of one or more consecutive squares
    in more than one way.
    This implementation generates all sums of consecutive squares
    in increasing order by using a min‑heap.  Whenever two equal sums
    are popped from the heap the value is stored in a static array.
    It works for any seq_t (built‑in unsigned integers or an arbitrary‑precision
    class) and any idx_t (e.g. uint64_t). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- data structures ----------------------------------------------------- */

typedef struct
{
    seq_t sum;       /* current sum of squares for this start */
    uint32_t a;      /* starting index of the run (>=1) */
    uint32_t b;      /* current end index of the run (>=a) */
} node_t;

/* The heap is a 1‑based array; heap[0] is unused. */
static node_t *heap = NULL;      /* binary min‑heap of node_t   */
static size_t   heap_size = 0;   /* number of elements in heap */

/* Array of found terms in increasing order. */
static seq_t *seq  = NULL;       /* Dynamically grown array */
static size_t seq_cap = 0;       /* capacity of seq   */
static size_t seq_len = 0;       /* number of stored elements */

/* --- helper functions --------------------------------------------------- */

static void heap_push(node_t v)
{
    size_t i = ++heap_size;
    while (i > 1 && heap[i >> 1].sum > v.sum)
    {
        heap[i] = heap[i >> 1];
        i >>= 1;
    }
    heap[i] = v;
}

static node_t heap_pop(void)
{
    node_t top   = heap[1];
    node_t last  = heap[heap_size--];
    size_t i = 1, child;
    while ((child = i << 1) <= heap_size)
    {
        if (child < heap_size && heap[child + 1].sum < heap[child].sum)
            child++;
        if (last.sum <= heap[child].sum)
            break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = last;
    return top;
}

/* ------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* static state ---------------------------------------------------- */
    static bool initialized = false;
    static seq_t prev_sum = 0;
    static uint32_t dupcnt = 0;
    static bool first = true;

    if (!initialized)
    {
        /* The value of MAX_A is chosen so that all terms needed for
           n <= 10000 are produced.  It can be increased if necessary. */
        const uint32_t MAX_A = 60000;

        heap = (node_t *) malloc((MAX_A + 1) * sizeof(node_t));
        heap_size = 0;

        for (uint32_t a = 1; a <= MAX_A; ++a)
        {
            seq_t s = (seq_t)a * (seq_t)a;   /* first sum for this a  */
            heap_push((node_t){s, a, a});
        }

        seq_cap = 16;
        seq = (seq_t *) malloc(seq_cap * sizeof(seq_t));

        initialized = true;
    }

    /* generate terms until we have at least n elements */
    while (seq_len < (size_t)n)
    {
        node_t nd      = heap_pop();
        seq_t s        = nd.sum;

        /* count duplicate sums (different a) */
        if (first)
        {
            first   = false;
            dupcnt  = 1;
            prev_sum = s;
        }
        else if (s == prev_sum)
        {
            ++dupcnt;
        }
        else
        {
            dupcnt = 1;
            prev_sum = s;
        }

        if (dupcnt == 2)               /* found a term for the first time */
        {
            if (seq_len >= seq_cap)
            {
                seq_cap = seq_cap * 2;
                seq = (seq_t *) realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = s;
        }

        /* push the next sum for the same starting a */
        uint32_t next_b   = nd.b + 1;
        seq_t next_sum    = nd.sum + ((seq_t)next_b * (seq_t)next_b);
        heap_push((node_t){next_sum, nd.a, next_b});
    }

    return seq[(size_t)n - 1];
}
